// Growatt Solar Inverter to MQTT
// Repo: https://github.com/nygma2004/growatt2mqtt
// author: Csongor Varga, csongor.varga@gmail.com
// 1 Phase, 2 string inverter version such as MIN 3000 TL-XE, MIC 1500 TL-X

// Libraries:
// - FastLED by Daniel Garcia
// - ModbusMaster by Doc Walker
// - ArduinoOTA
// - SoftwareSerial
// Hardware:
// - Wemos D1 mini
// - RS485 to TTL converter: https://www.aliexpress.com/item/1005001621798947.html
// - To power from mains: Hi-Link 5V power supply (https://www.aliexpress.com/item/1005001484531375.html), fuseholder and 1A fuse, and varistor


#include <SoftwareSerial.h>       // Leave the main serial line (USB) for debugging and flashing
#include <ModbusMaster.h>         // Modbus master library for ESP8266
#include <ESP8266WiFi.h>          // Wifi connection
#include <ESP8266WebServer.h>     // Web server for general HTTP response
#include <PubSubClient.h>         // MQTT support
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FastLED.h>
#include "globals.h"
#include "settings.h"



os_timer_t myTimer;
ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient mqtt(mqtt_server, 1883, 0, espClient);
// SoftwareSerial modbus(MAX485_RX, MAX485_TX, false, 256); //RX, TX
SoftwareSerial modbus(MAX485_RX, MAX485_TX, false); //RX, TX
ModbusMaster growatt;
CRGB leds[NUM_LEDS];

void preTransmission() {
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void sendModbusError(uint8_t result) {
  String message = "";
  if (result==growatt.ku8MBIllegalFunction) {
    message = "Illegal function";
  }
  if (result==growatt.ku8MBIllegalDataAddress) {
    message = "Illegal data address";
  }
  if (result==growatt.ku8MBIllegalDataValue) {
    message = "Illegal data value";
  }
  if (result==growatt.ku8MBSlaveDeviceFailure) {
    message = "Slave device failure";
  }
  if (result==growatt.ku8MBInvalidSlaveID) {
    message = "Invalid slave ID";
  }
  if (result==growatt.ku8MBInvalidFunction) {
    message = "Invalid function";
  }
  if (result==growatt.ku8MBResponseTimedOut) {
    message = "Response timed out";
  }
  if (result==growatt.ku8MBInvalidCRC) {
    message = "Invalid CRC";
  }
  if (message=="") {
    message = result;
  }
  Serial.println(message);    
  char topic[80];
  char value[30];
  sprintf(topic,"%s/error",topicRoot);
  mqtt.publish(topic, message.c_str());
  delay(5);
}

void ReadInputRegisters() {
  char json[1024];
  char topic[80];
  char value[10]; 

  leds[0] = CRGB::Yellow;
  FastLED.show();
  uint8_t result;

  digitalWrite(STATUS_LED, 0);

  ESP.wdtDisable();
  result = growatt.readInputRegisters(setcounter*64,64);
  ESP.wdtEnable(1);
  if (result == growatt.ku8MBSuccess)   {

    leds[0] = CRGB::Green;
    FastLED.show();
    lastRGB = millis();
    ledoff = true;
    
    #ifdef DEBUG_SERIAL
      Serial.print("InputReg page");
      Serial.print(setcounter);
      Serial.print(": ");
    #endif
    sprintf(json,"{");
    for(int i=0;i<64;i++) {

      #ifdef DEBUG_SERIAL
        sprintf(value,"%d|",growatt.getResponseBuffer(i));
        Serial.print(value);
      #endif
      #ifdef DEBUG_MQTT
        sprintf(json,"%s \"r%d\":%d,",json,i,growatt.getResponseBuffer(i));
      #endif
      
    }
    sprintf(json,"%s \"end\":1 }",json);

    #ifdef DEBUG_SERIAL
      Serial.println();
    #endif
    #ifdef DEBUG_MQTT
      sprintf(topic,"%s/raw",topicRoot);
      mqtt.publish(topic, json);
    #endif

    if (setcounter==0) {
      // Status and PV data
      modbusdata.status = growatt.getResponseBuffer(0);
      modbusdata.solarpower = ((growatt.getResponseBuffer(1) << 16) | growatt.getResponseBuffer(2))*0.1;
      
      modbusdata.pv1voltage = growatt.getResponseBuffer(3)*0.1;
      modbusdata.pv1current = growatt.getResponseBuffer(4)*0.1;
      modbusdata.pv1power = ((growatt.getResponseBuffer(5) << 16) | growatt.getResponseBuffer(6))*0.1;
  
      modbusdata.pv2voltage = growatt.getResponseBuffer(7)*0.1;
      modbusdata.pv2current = growatt.getResponseBuffer(8)*0.1;
      modbusdata.pv2power = ((growatt.getResponseBuffer(9) << 16) | growatt.getResponseBuffer(10))*0.1;
  
      // Output
      modbusdata.outputpower = ((growatt.getResponseBuffer(35) << 16) | growatt.getResponseBuffer(36))*0.1;
      modbusdata.gridfrequency = growatt.getResponseBuffer(37)*0.01;
      modbusdata.gridvoltage = growatt.getResponseBuffer(38)*0.1;
  
      // Energy
      modbusdata.energytoday = ((growatt.getResponseBuffer(53) << 16) | growatt.getResponseBuffer(54))*0.1;
      modbusdata.energytotal = ((growatt.getResponseBuffer(55) << 16) | growatt.getResponseBuffer(56))*0.1;
      modbusdata.totalworktime = ((growatt.getResponseBuffer(57) << 16) | growatt.getResponseBuffer(58))*0.5;
  
      modbusdata.pv1energytoday = ((growatt.getResponseBuffer(59) << 16) | growatt.getResponseBuffer(60))*0.1;
      modbusdata.pv1energytotal = ((growatt.getResponseBuffer(61) << 16) | growatt.getResponseBuffer(62))*0.1;
      overflow = growatt.getResponseBuffer(63);
    }
    if (setcounter==1) {
      modbusdata.pv2energytoday = ((overflow << 16) | growatt.getResponseBuffer(64-64))*0.1;
      modbusdata.pv2energytotal = ((growatt.getResponseBuffer(65-64) << 16) | growatt.getResponseBuffer(66-64))*0.1;
  
      // Temperatures
      modbusdata.tempinverter = growatt.getResponseBuffer(93-64)*0.1;
      modbusdata.tempipm = growatt.getResponseBuffer(94-64)*0.1;
      modbusdata.tempboost = growatt.getResponseBuffer(95-64)*0.1;
  
      // Diag data
      modbusdata.ipf = growatt.getResponseBuffer(100-64);
      modbusdata.realoppercent = growatt.getResponseBuffer(101-64);
      modbusdata.opfullpower = ((growatt.getResponseBuffer(102-64) << 16) | growatt.getResponseBuffer(103-64))*0.1;
      modbusdata.deratingmode = growatt.getResponseBuffer(103-64);
                                //  0:no derate;
                                //  1:PV;
                                //  2:*;
                                //  3:Vac;
                                //  4:Fac;
                                //  5:Tboost;
                                //  6:Tinv;
                                //  7:Control;
                                //  8:*;
                                //  9:*OverBack
                                //  ByTime;
      
      modbusdata.faultcode = growatt.getResponseBuffer(105-64);
                            //  1~23 " Error: 99+x
                            //  24 "Auto Test
                            //  25 "No AC
                            //  26 "PV Isolation Low",
                            //  27 " Residual I
                            //  28 " Output High
                            //  29 " PV Voltage
                            //  30 " AC V Outrange
                            //  31 " AC F Outrange
                            //  32 " Module Hot
  
  
      modbusdata.faultbitcode = ((growatt.getResponseBuffer(105-64) << 16) | growatt.getResponseBuffer(106-64));
                                //  0x00000001 \
                                //  0x00000002 Communication error
                                //  0x00000004 \
                                //  0x00000008 StrReverse or StrShort fault
                                //  0x00000010 Model Init fault
                                //  0x00000020 Grid Volt Sample diffirent
                                //  0x00000040 ISO Sample diffirent
                                //  0x00000080 GFCI Sample diffirent
                                //  0x00000100 \
                                //  0x00000200 \
                                //  0x00000400 \
                                //  0x00000800 \
                                //  0x00001000 AFCI Fault
                                //  0x00002000 \
                                //  0x00004000 AFCI Module fault
                                //  0x00008000 \
                                //  0x00010000 \
                                //  0x00020000 Relay check fault
                                //  0x00040000 \
                                //  0x00080000 \
                                //  0x00100000 \
                                //  0x00200000 Communication error
                                //  0x00400000 Bus Voltage error
                                //  0x00800000 AutoTest fail
                                //  0x01000000 No Utility
                                //  0x02000000 PV Isolation Low
                                //  0x04000000 Residual I High
                                //  0x08000000 Output High DCI
                                //  0x10000000 PV Voltage high
                                //  0x20000000 AC V Outrange
                                //  0x40000000 AC F Outrange
                                //  0x80000000 TempratureHigh
  
      modbusdata.warningbitcode = ((growatt.getResponseBuffer(110-64) << 16) | growatt.getResponseBuffer(111-64));
                                  //  0x0001 Fan warning
                                  //  0x0002 String communication abnormal
                                  //  0x0004 StrPIDconfig Warning
                                  //  0x0008 \
                                  //  0x0010 DSP and COM firmware unmatch
                                  //  0x0020 \
                                  //  0x0040 SPD abnormal
                                  //  0x0080 GND and N connect abnormal
                                  //  0x0100 PV1 or PV2 circuit short
                                  //  0x0200 PV1 or PV2 boost driver broken
                                  //  0x0400 \
                                  //  0x0800 \
                                  //  0x1000 \
                                  //  0x2000 \
                                  //  0x4000 \
                                  //  0x8000 \
                                  
      }

      setcounter++;
      if (setcounter==2) {
        setcounter = 0;      
    
        // Generate the modbus MQTT message
        sprintf(json,"{",json);
        sprintf(json,"%s \"status\":%d,",json,modbusdata.status);
        sprintf(json,"%s \"solarpower\":%.1f,",json,modbusdata.solarpower);
        sprintf(json,"%s \"pv1voltage\":%.1f,",json,modbusdata.pv1voltage);
        sprintf(json,"%s \"pv1current\":%.1f,",json,modbusdata.pv1current);
        sprintf(json,"%s \"pv1power\":%.1f,",json,modbusdata.pv1power);
        sprintf(json,"%s \"pv2voltage\":%.1f,",json,modbusdata.pv2voltage);
        sprintf(json,"%s \"pv2current\":%.1f,",json,modbusdata.pv2current);
        sprintf(json,"%s \"pv2power\":%.1f,",json,modbusdata.pv2power);
        
        sprintf(json,"%s \"outputpower\":%.1f,",json,modbusdata.outputpower);
        sprintf(json,"%s \"gridfrequency\":%.2f,",json,modbusdata.gridfrequency);
        sprintf(json,"%s \"gridvoltage\":%.1f,",json,modbusdata.gridvoltage);
    
        sprintf(json,"%s \"energytoday\":%.1f,",json,modbusdata.energytoday);
        sprintf(json,"%s \"energytotal\":%.1f,",json,modbusdata.energytotal);
        sprintf(json,"%s \"totalworktime\":%.1f,",json,modbusdata.totalworktime);
        sprintf(json,"%s \"pv1energytoday\":%.1f,",json,modbusdata.pv1energytoday);
        sprintf(json,"%s \"pv1energytotal\":%.1f,",json,modbusdata.pv1energytotal);
        sprintf(json,"%s \"pv2energytoday\":%.1f,",json,modbusdata.pv2energytoday);
        sprintf(json,"%s \"pv2energytotal\":%.1f,",json,modbusdata.pv2energytotal);
        sprintf(json,"%s \"opfullpower\":%.1f,",json,modbusdata.opfullpower);
    
        sprintf(json,"%s \"tempinverter\":%.1f,",json,modbusdata.tempinverter);
        sprintf(json,"%s \"tempipm\":%.1f,",json,modbusdata.tempipm);
        sprintf(json,"%s \"tempboost\":%.1f,",json,modbusdata.tempboost);
    
        sprintf(json,"%s \"ipf\":%d,",json,modbusdata.ipf);
        sprintf(json,"%s \"realoppercent\":%d,",json,modbusdata.realoppercent);
        sprintf(json,"%s \"deratingmode\":%d,",json,modbusdata.deratingmode);
        sprintf(json,"%s \"faultcode\":%d,",json,modbusdata.faultcode);
        sprintf(json,"%s \"faultbitcode\":%d,",json,modbusdata.faultbitcode);
        sprintf(json,"%s \"warningbitcode\":%d }",json,modbusdata.warningbitcode);
  
        #ifdef DEBUG_SERIAL
        Serial.println(json);
        #endif
        sprintf(topic,"%s/data",topicRoot);
        mqtt.publish(topic,json);      
        Serial.println("Data MQTT sent");
    }
    //sprintf(topic,"%s/error",topicRoot);
    //mqtt.publish(topic,"OK");

  } else {
    leds[0] = CRGB::Red;
    FastLED.show();
    lastRGB = millis();
    ledoff = true;

    Serial.print(F("Error: "));
    sendModbusError(result);
  }
  digitalWrite(STATUS_LED, 1);



    
}

void ReadHoldingRegisters() {
  char json[1024];
  char topic[80];
  char value[10]; 


  leds[0] = CRGB::Yellow;
  FastLED.show();
  uint8_t result;
  //uint16_t data[6];

  digitalWrite(STATUS_LED, 0);
  ESP.wdtDisable();
  result = growatt.readHoldingRegisters(setcounter*64,64);
  ESP.wdtEnable(1);
  if (result == growatt.ku8MBSuccess)   {

    leds[0] = CRGB::Green;
    FastLED.show();
    lastRGB = millis();
    ledoff = true;
    
    #ifdef DEBUG_SERIAL
      Serial.print("HoldingReg Page");
      Serial.print(setcounter);
      Serial.print(": ");
    #endif
    sprintf(json,"{");
    for(int i=0;i<64;i++) {

      #ifdef DEBUG_SERIAL
        sprintf(value,"%d|",growatt.getResponseBuffer(i));
        Serial.print(value);
      #endif
      #ifdef DEBUG_MQTT
        sprintf(json,"%s \"r%d\":%d,",json,i,growatt.getResponseBuffer(i));
      #endif
      
    }
    sprintf(json,"%s \"end\":1 }",json);

    #ifdef DEBUG_SERIAL
      Serial.println();
    #endif
    #ifdef DEBUG_MQTT
      sprintf(topic,"%s/holding",topicRoot);
      mqtt.publish(topic, json);
    #endif

    if (setcounter==0) {
      modbussettings.safetyfuncen = growatt.getResponseBuffer(1); // Safety Function Enabled
                                  //  Bit0: SPI enable
                                  //  Bit1: AutoTestStart
                                  //  Bit2: LVFRT enable
                                  //  Bit3: FreqDerating Enable
                                  //  Bit4: Softstart enable
                                  //  Bit5: DRMS enable
                                  //  Bit6: Power Volt Func Enable
                                  //  Bit7: HVFRT enable
                                  //  Bit8: ROCOF enable
                                  //  Bit9: Recover FreqDerating Mode Enable
                                  //  Bit10~15: Reserved
      modbussettings.maxoutputactivepp = growatt.getResponseBuffer(3); // Inverter M ax output active power percent  0-100: %, 255: not limited
      modbussettings.maxoutputreactivepp = growatt.getResponseBuffer(4); // Inverter M ax output reactive power percent  0-100: %, 255: not limited
      modbussettings.maxpower = ((growatt.getResponseBuffer(6) << 16) | growatt.getResponseBuffer(7))*0.1;
      modbussettings.voltnormal = growatt.getResponseBuffer(8)*0.1;
      strncpy(modbussettings.firmware,"      ",6);
      modbussettings.firmware[0] = growatt.getResponseBuffer(9) >> 8;
      modbussettings.firmware[1] = growatt.getResponseBuffer(9) & 0xff;
      modbussettings.firmware[2] = growatt.getResponseBuffer(10) >> 8;
      modbussettings.firmware[3] = growatt.getResponseBuffer(10) & 0xff;
      modbussettings.firmware[4] = growatt.getResponseBuffer(11) >> 8;
      modbussettings.firmware[5] = growatt.getResponseBuffer(11) & 0xff;
  
      strncpy(modbussettings.controlfirmware,"      ",6);
      modbussettings.controlfirmware[0] = growatt.getResponseBuffer(12) >> 8;
      modbussettings.controlfirmware[1] = growatt.getResponseBuffer(12) & 0xff;
      modbussettings.controlfirmware[2] = growatt.getResponseBuffer(13) >> 8;
      modbussettings.controlfirmware[3] = growatt.getResponseBuffer(13) & 0xff;
      modbussettings.controlfirmware[4] = growatt.getResponseBuffer(14) >> 8;
      modbussettings.controlfirmware[5] = growatt.getResponseBuffer(14) & 0xff;
      
      modbussettings.startvoltage = growatt.getResponseBuffer(17)*0.1;
  
      strncpy(modbussettings.serial,"          ",10);
      modbussettings.serial[0] = growatt.getResponseBuffer(23) >> 8;
      modbussettings.serial[1] = growatt.getResponseBuffer(23) & 0xff;
      modbussettings.serial[2] = growatt.getResponseBuffer(24) >> 8;
      modbussettings.serial[3] = growatt.getResponseBuffer(24) & 0xff;
      modbussettings.serial[4] = growatt.getResponseBuffer(25) >> 8;
      modbussettings.serial[5] = growatt.getResponseBuffer(25) & 0xff;
      modbussettings.serial[6] = growatt.getResponseBuffer(26) >> 8;
      modbussettings.serial[7] = growatt.getResponseBuffer(26) & 0xff;
      modbussettings.serial[8] = growatt.getResponseBuffer(27) >> 8;
      modbussettings.serial[9] = growatt.getResponseBuffer(27) & 0xff;
   
      modbussettings.gridvoltlowlimit = growatt.getResponseBuffer(52)*0.1;
      modbussettings.gridvolthighlimit = growatt.getResponseBuffer(53)*0.1;
      modbussettings.gridfreqlowlimit = growatt.getResponseBuffer(54)*0.01;
      modbussettings.gridfreqhighlimit = growatt.getResponseBuffer(55)*0.01;
    }
    if (setcounter==1) {
      modbussettings.gridvoltlowconnlimit = growatt.getResponseBuffer(64-64)*0.1;
      modbussettings.gridvolthighconnlimit = growatt.getResponseBuffer(65-64)*0.1;
      modbussettings.gridfreqlowconnlimit = growatt.getResponseBuffer(66-64)*0.01;
      modbussettings.gridfreqhighconnlimit = growatt.getResponseBuffer(67-64)*0.01;
    }

    setcounter++;
    if (setcounter==2) {
      setcounter = 0;
      // Generate the modbus MQTT message
      sprintf(json,"{",json);
  
      sprintf(json,"%s \"safetyfuncen\":%d,",json,modbussettings.safetyfuncen);
      sprintf(json,"%s \"maxoutputactivepp\":%d,",json,modbussettings.maxoutputactivepp);
      sprintf(json,"%s \"maxoutputreactivepp\":%d,",json,modbussettings.maxoutputreactivepp);
  
      sprintf(json,"%s \"maxpower\":%.1f,",json,modbussettings.maxpower);
      sprintf(json,"%s \"voltnormal\":%.1f,",json,modbussettings.voltnormal);
      sprintf(json,"%s \"startvoltage\":%.1f,",json,modbussettings.startvoltage);
      sprintf(json,"%s \"gridvoltlowlimit\":%.1f,",json,modbussettings.gridvoltlowlimit);
      sprintf(json,"%s \"gridvolthighlimit\":%.1f,",json,modbussettings.gridvolthighlimit);
      sprintf(json,"%s \"gridfreqlowlimit\":%.1f,",json,modbussettings.gridfreqlowlimit);
      sprintf(json,"%s \"gridfreqhighlimit\":%.1f,",json,modbussettings.gridfreqhighlimit);
      sprintf(json,"%s \"gridvoltlowconnlimit\":%.1f,",json,modbussettings.gridvoltlowconnlimit);
      sprintf(json,"%s \"gridvolthighconnlimit\":%.1f,",json,modbussettings.gridvolthighconnlimit);
      sprintf(json,"%s \"gridfreqlowconnlimit\":%.1f,",json,modbussettings.gridfreqlowconnlimit);
      sprintf(json,"%s \"gridfreqhighconnlimit\":%.1f,",json,modbussettings.gridfreqhighconnlimit);
  
      sprintf(json,"%s \"firmware\":\"%s\",",json,modbussettings.firmware);
      sprintf(json,"%s \"controlfirmware\":\"%s\",",json,modbussettings.controlfirmware);
      sprintf(json,"%s \"serial\":\"%s\" }",json,modbussettings.serial);

      #ifdef DEBUG_SERIAL
      Serial.println(json);
      #endif
      sprintf(topic,"%s/settings",topicRoot);
      mqtt.publish(topic,json);      
      Serial.println("Setting MQTT sent");
      // Set the flag to true not to read the holding registers again
      holdingregisters=true;
    }
    //sprintf(topic,"%s/error",topicRoot);
    //mqtt.publish(topic,"OK");


  } else {
    leds[0] = CRGB::Red;
    FastLED.show();
    lastRGB = millis();
    ledoff = true;

    Serial.print(F("Error: "));
    sendModbusError(result);
  }
  digitalWrite(STATUS_LED, 1);


    
}



// This is the 1 second timer callback function
void timerCallback(void *pArg) {

  
  seconds++;

  // Query the modbus device 
  if (seconds % UPDATE_MODBUS==0) {
    //ReadInputRegisters();
    if (!holdingregisters) {
      // Read the holding registers
      ReadHoldingRegisters();      
    } else {
      // Read the input registers
      ReadInputRegisters();
    }
  }

  // Send RSSI and uptime status
  if (seconds % UPDATE_STATUS==0) {
    // Send MQTT update
    if (mqtt_server!="") {
      char topic[80];
      char value[300];
      sprintf(value,"{\"rssi\": %d, \"uptime\": %d, \"ssid\": \"%s\", \"ip\": \"%d.%d.%d.%d\", \"clientid\":\"%s\", \"version\":\"%s\"}",WiFi.RSSI(),uptime,WiFi.SSID().c_str(),WiFi.localIP()[0],WiFi.localIP()[1],WiFi.localIP()[2],WiFi.localIP()[3],newclientid,buildversion);
      sprintf(topic,"%s/%s",topicRoot,"status");
      mqtt.publish(topic, value);
      Serial.println(F("MQTT status sent"));
    }
  }


}

// MQTT reconnect logic
void reconnect() {
  //String mytopic;
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    byte mac[6];                     // the MAC address of your Wifi shield
    WiFi.macAddress(mac);
    sprintf(newclientid,"%s-%02x%02x%02x",clientID,mac[2],mac[1],mac[0]);
    Serial.print(F("Client ID: "));
    Serial.println(newclientid);
    // Attempt to connect
    if (mqtt.connect(newclientid, mqtt_user, mqtt_password)) {
      Serial.println(F("connected"));
      // ... and resubscribe
      char topic[80];
      sprintf(topic,"%swrite/#",topicRoot);
      mqtt.subscribe(topic);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(mqtt.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {

  FastLED.addLeds<LED_TYPE, RGBLED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );
  leds[0] = CRGB::Pink;
  FastLED.show();

  Serial.begin(SERIAL_RATE);
  Serial.println(F("\nGrowatt Solar Inverter to MQTT Gateway"));
  // Init outputs, RS485 in receive mode
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Initialize some variables
  uptime = 0;
  seconds = 0;
  leds[0] = CRGB::Pink;
  FastLED.show();

  // Connect to Wifi
  Serial.print(F("Connecting to Wifi"));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
    seconds++;
    if (seconds>180) {
      // reboot the ESP if cannot connect to wifi
      ESP.restart();
    }
  }
  seconds = 0;
  Serial.println("");
  Serial.println(F("Connected to wifi network"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("Signal [RSSI]: "));
  Serial.println(WiFi.RSSI());

  // Set up the Modbus line
  growatt.begin(SLAVE_ID , modbus);
  // Callbacks allow us to configure the RS485 transceiver correctly
  growatt.preTransmission(preTransmission);
  growatt.postTransmission(postTransmission);
  Serial.println("Modbus connection is set up");

  // Create the 1 second timer interrupt
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 1000, true);

  server.on("/", [](){                        // Dummy page
    server.send(200, "text/plain", "Growatt Solar Inverter to MQTT Gateway");
  });
  server.begin();
  Serial.println(F("HTTP server started"));

  // Set up the MQTT server connection
  if (mqtt_server!="") {
    mqtt.setServer(mqtt_server, 1883);
    mqtt.setBufferSize(1024);
    mqtt.setCallback(callback);
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  byte mac[6];                     // the MAC address of your Wifi shield
  WiFi.macAddress(mac);
  char value[80];
  sprintf(value,"%s-%02x%02x%02x",clientID,mac[2],mac[1],mac[0]);
  ArduinoOTA.setHostname(value);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  modbus.begin(MODBUS_RATE);
  
  leds[0] = CRGB::Black;
  FastLED.show();
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Convert the incoming byte array to a string
  String mytopic = (char*)topic;
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;

  Serial.print(F("Message arrived on topic: ["));
  Serial.print(topic);
  Serial.print(F("], "));
  Serial.println(message);


}

void loop() {
  // Handle HTTP server requests
  server.handleClient();
  ArduinoOTA.handle();

  // Handle MQTT connection/reconnection
  if (mqtt_server!="") {
    if (!mqtt.connected()) {
      reconnect();
    }
    mqtt.loop();
  }

  // Uptime calculation
  if (millis() - lastTick >= 60000) {            
    lastTick = millis();            
    uptime++;            
  }    

  if (millis() - lastWifiCheck >= WIFICHECK) {
    // reconnect to the wifi network if connection is lost
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconnecting to wifi...");
      WiFi.reconnect();
    }
    lastWifiCheck = millis();
  }

  if (ledoff && (millis() - lastRGB >= RGBSTATUSDELAY)) {
    ledoff = false;
    leds[0] = CRGB::Black;
    FastLED.show();
  }


}
