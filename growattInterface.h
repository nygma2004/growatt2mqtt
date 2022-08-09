#ifndef GROWATTINTERFACE_H
#define GROWATTINTERFACE_H

#include "Arduino.h"
#include <ModbusMaster.h>         // Modbus master library for ESP8266
#include <SoftwareSerial.h>       // Leave the main serial line (USB) for debugging and flashing


class growattIF {
  #define SLAVE_ID        1         // Default slave ID of Growatt
  #define MODBUS_RATE     9600      // Modbus speed of Growatt, do not change
  
  private:
    ModbusMaster growattInterface;
    SoftwareSerial *serial;
    void preTransmission();
    void postTransmission();
    int PinMAX485_RE_NEG;
    int PinMAX485_DE;
    int PinMAX485_RX;
    int PinMAX485_TX;
    int setcounter = 0;
    int overflow;

    struct modbus_input_registers
    {
      int status;
      float solarpower, pv1voltage, pv1current, pv1power, pv2voltage, pv2current, pv2power, outputpower, gridfrequency, gridvoltage;
      float energytoday, energytotal, totalworktime, pv1energytoday, pv1energytotal, pv2energytoday, pv2energytotal, opfullpower;
      float tempinverter, tempipm, tempboost;
      int ipf, realoppercent, deratingmode, faultcode, faultbitcode, warningbitcode;
    };
    struct modbus_input_registers modbusdata;

    struct modbus_holding_registers
    {
      int enable, safetyfuncen, maxoutputactivepp, maxoutputreactivepp;
      float  maxpower, voltnormal, startvoltage, gridvoltlowlimit, gridvolthighlimit, gridfreqlowlimit, gridfreqhighlimit, gridvoltlowconnlimit, gridvolthighconnlimit, gridfreqlowconnlimit, gridfreqhighconnlimit;
      char firmware[6], controlfirmware[6];
      char serial[10];
    };

    struct modbus_holding_registers modbussettings;
  public:
    growattIF(int _PinMAX485_RE_NEG, int _PinMAX485_DE, int _PinMAX485_RX, int _PinMAX485_TX);
    void initGrowatt();
    void writeRegister8Bit(int reg, int message);
    uint8_t readRegister8Bit(int reg);
    uint8_t ReadInputRegisters(char* json);
    uint8_t ReadHoldingRegisters(char* json);
    String sendModbusError(uint8_t result);

    // Error codes
    static const uint8_t Success    = 0x00;
    static const uint8_t Continue   = 0xFF;
    
    // Growatt Holding registers
    static const uint8_t regOnOff           = 0;
    static const uint8_t regMaxOutputActive = 3;
    static const uint8_t regStartVoltage    = 17;
};

#endif
