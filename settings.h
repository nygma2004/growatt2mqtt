#pragma once
//#define DEBUG_SERIAL    1
//#define DEBUG_MQTT      1 
//#define useModulPower   1
#define HA_ENABLE_DISCOVERY 1

#define SERIAL_RATE     115200    // Serial speed for status info
#define MAX485_DE       5         // D1, DE pin on the TTL to RS485 converter
#define MAX485_RE_NEG   4         // D2, RE pin on the TTL to RS485 converter
#define MAX485_RX       14        // D5, RO pin on the TTL to RS485 converter
#define MAX485_TX       12        // D6, DI pin on the TTL to RS485 converter
#define STATUS_LED      2         // Status LED on the Wemos D1 mini (D4)
#define UPDATE_MODBUS   2         // 1: modbus device is read every second
#define UPDATE_STATUS   30        // 10: status mqtt message is sent every 10 seconds
#define RGBSTATUSDELAY  500       // delay for turning off the status led
#define WIFICHECK       500       // how often check lost wifi connection

#define RGBLED_PIN D3        // Neopixel led D3
#define NUM_LEDS 1
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS  64        // Default LED brightness.

// Update the below parameters for your project
// Also check NTP.h for some parameters as well
static const char* ssid = "xxxx";           // Wifi SSID
static const char* password = "xxxx";    // Wifi password
static const char* mqtt_server = "192.168.x.xx";     // MQTT server
static const char* mqtt_user = "xxxx";             // MQTT userid
static const char* mqtt_password = "xxxx";         // MQTT password
static const char* clientID = "growatt";                // MQTT client ID
static const char* topicRoot = "growatt";             // MQTT root topic for the device, keep / at the end


// Comment the entire second below for dynamic IP (including the define)
// #define FIXEDIP   1
static const IPAddress local_IP(192, 168, 1, 205);         // Set your Static IP address
static const IPAddress gateway(192, 168, 1, 254);          // Set your Gateway IP address
static const IPAddress subnet(255, 255, 255, 0);
static const IPAddress primaryDNS(192, 168, 1, 254);   //optional
static const IPAddress secondaryDNS(8, 8, 4, 4); //optional

// #define ZeroExport 1
#define UPDATE_SMETER 5                     //Update smart meter data every 5 seconds
static const int SmartMeterOffset = 0;            // Watt offset to apply to smart meter reading (to account for baseline consumption, set to 0 if not needed)
static const char* SmartMeterEndpoint = "http://192.168.X.XXX/cm?cmnd=STATUS%2010";
static const char* SmartMeterKey = "Power";        // JSON key to read from smart meter response
