#include "zeroExport.h"
#include "settings.h"

// Constructor
zeroExport::zeroExport(const char* _endpoint, uint32_t _updateInterval)
  : endpoint(_endpoint), updateInterval(_updateInterval * 1000), lastUpdate(millis()), isRunning(true) {
  Serial.println(F("zeroExport started"));
}

// Destructor
zeroExport::~zeroExport() {
  stop();
}

// Stop the meter reader
void zeroExport::stop() {
  if (isRunning) {
    isRunning = false;
    Serial.println(F("zeroExport stopped"));
  }
}

// Handle method - call this from main loop
void zeroExport::handle(char dataJson[1024], char settingsJson[1024], uint8_t *output) {

  if (!isRunning) {
    return;              // Not running, do nothing
  }
   
  // Check if it's time to update
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    int smartmeter;
    float solarpower;
    float maxpower;

    if (readSmartMeter(&smartmeter) != 0) {
     *output = 100;  // Default to 100% output if smart meter reading fails
      return;
    }

    DeserializationError err = deserializeJson(doc, dataJson);
    solarpower = doc["outputpower"].as<float>();
    doc.clear();
    err = deserializeJson(doc, settingsJson);
    maxpower = doc["maxpower"].as<float>();
    
    float outPercent = 0.0f;
    if (maxpower > 0.0f) {
      outPercent = ((solarpower + (float)smartmeter - (float)SmartMeterOffset) / maxpower) * 100.0f;
    }
    if (outPercent < 0.0f) outPercent = 0.0f;
    if (outPercent > 100.0f) outPercent = 100.0f;
    *output = (uint8_t)(outPercent + 0.5f);

    #ifdef DEBUG_SERIAL
    Serial.printf("Smartmeter: %d\r\n", smartmeter);
    Serial.print(F("Solarpower: "));
    Serial.println(solarpower);
    Serial.print(F("Maxpower: "));
    Serial.println(maxpower);
    Serial.print(F("Output: "));
    Serial.println(*output);
    #endif   
  }
}

static int searchPower(JsonVariant var) {
  if (var.is<JsonObject>()) {
    JsonObject obj = var.as<JsonObject>();
    if (obj[SmartMeterKey].is<int>()) {
      return obj[SmartMeterKey].as<int>();
    }
    for (JsonPair kv : obj) {
      int v = searchPower(kv.value());
      if (v != INT_MIN) return v;
    }
  } else if (var.is<JsonArray>()) {
    for (JsonVariant el : var.as<JsonArray>()) {
      int v = searchPower(el);
      if (v != INT_MIN) return v;
    }
  }
  return INT_MIN;
}

int zeroExport::readSmartMeter(int *result) {
  // Perform the HTTP request
    if (httpClient.begin(wifiClient, endpoint)) {
      int httpCode = httpClient.GET();
      if (httpCode == HTTP_CODE_OK) {
        String payload = httpClient.getString();
        DeserializationError err = deserializeJson(doc, payload);
        if (!err) {
            int p = searchPower(doc.as<JsonVariant>());
            if (p != INT_MIN) *result = p;
        }
      } else {
        Serial.printf("[SmartMeterReader] HTTP-Error: %s\n", httpClient.errorToString(httpCode).c_str());
        return httpCode;
      }
      httpClient.end();
      return 0;
    } else {
      Serial.println(F("[SmartMeterReader] Failed to connect to endpoint"));
      return -1;
    }
}