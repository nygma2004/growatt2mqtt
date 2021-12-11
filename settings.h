//#define DEBUG_SERIAL    1
//#define DEBUG_MQTT      1 
#define MODBUS_RATE     9600      // Modbus speed of Growatt, do not change
#define SERIAL_RATE     115200    // Serial speed for status info
#define MAX485_DE       5         // D1, DE pin on the TTL to RS485 converter
#define MAX485_RE_NEG   4         // D2, RE pin on the TTL to RS485 converter
#define MAX485_RX       14        // D5, RO pin on the TTL to RS485 converter
#define MAX485_TX       12        // D6, DI pin on the TTL to RS485 converter
#define SLAVE_ID        1         // Default slave ID of Growatt
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
const char* ssid = "xxxx";           // Wifi SSID
const char* password = "xxxx";    // Wifi password
const char* mqtt_server = "192.168.x.xx";     // MQTT server
const char* mqtt_user = "xxxx";             // MQTT userid
const char* mqtt_password = "xxxx";         // MQTT password
const char* clientID = "growatt";                // MQTT client ID
const char* topicRoot = "growatt";             // MQTT root topic for the device, keep / at the end


// Comment the entire second below for dynamic IP (including the define)
// #define FIXEDIP   1
IPAddress local_IP(192, 168, 1, 205);         // Set your Static IP address
IPAddress gateway(192, 168, 1, 254);          // Set your Gateway IP address
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 1, 254);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional