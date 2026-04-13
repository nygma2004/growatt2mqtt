#ifndef ZEROEXPORT_H
#define ZEROEXPORT_H
#endif

#include "Arduino.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class zeroExport {
  private:
    HTTPClient httpClient;
    WiFiClient wifiClient;
    const char* endpoint;
    uint32_t updateInterval;  // in milliseconds
    unsigned long lastUpdate;
    bool isRunning;
    DynamicJsonDocument doc{32};
    
  public:
    zeroExport(const char* _endpoint, uint32_t _updateInterval = 5);
    ~zeroExport();
    
    void stop();
    void handle(char dataJson[1024], char settingsJson[1024], uint8_t *output);  // Call from main loop - performs the HTTP request if interval has passed
    int readSmartMeter(int *result);
  };