#include "growattInterface.h"

growattIF::growattIF(int _PinMAX485_RE_NEG, int _PinMAX485_DE, int _PinMAX485_RX, int _PinMAX485_TX) {
  PinMAX485_RE_NEG = _PinMAX485_RE_NEG;
  PinMAX485_DE = _PinMAX485_DE;
  PinMAX485_RX = _PinMAX485_RX;
  PinMAX485_TX = _PinMAX485_TX;
  
  // Init outputs, RS485 in receive mode
   pinMode(PinMAX485_RE_NEG, OUTPUT);
   pinMode(PinMAX485_DE, OUTPUT);
   digitalWrite(PinMAX485_RE_NEG, 0);
   digitalWrite(PinMAX485_DE, 0);
}

void growattIF::initGrowatt() {
  serial = new SoftwareSerial (PinMAX485_RX, PinMAX485_TX, false); //RX, TX
  serial->begin(MODBUS_RATE);
  growattInterface.begin(SLAVE_ID , *serial);

  static growattIF* obj = this;                               //pointer to the object
  // Callbacks allow us to configure the RS485 transceiver correctly
  growattInterface.preTransmission ([]() {                   //Set function pointer via anonymous Lambda function
    obj->preTransmission();
  });

  growattInterface.postTransmission([]() {                   //Set function pointer via anonymous Lambda function
    obj->postTransmission();
  });
}

uint8_t growattIF::writeRegister8Bit(int reg, int message) {
  return growattInterface.writeSingleRegister(reg, message);
}

uint8_t growattIF::readRegister8Bit(int reg){
  growattInterface.readHoldingRegisters(reg, 1);
  return growattInterface.getResponseBuffer(0);
}

void growattIF::preTransmission() {
  digitalWrite(PinMAX485_RE_NEG, 1);
  digitalWrite(PinMAX485_DE, 1);
}

void growattIF::postTransmission() {
  digitalWrite(PinMAX485_RE_NEG, 0);
  digitalWrite(PinMAX485_DE, 0);
}

uint8_t growattIF::ReadInputRegisters(char* json) {
  uint8_t result;

  ESP.wdtDisable();
  result = growattInterface.readInputRegisters(setcounter * 64, 64);
  ESP.wdtEnable(1);

  if (result == growattInterface.ku8MBSuccess)   {
    if (setcounter == 0) {
      // Status and PV data
      modbusdata.status = growattInterface.getResponseBuffer(0);
      modbusdata.solarpower = ((growattInterface.getResponseBuffer(1) << 16) | growattInterface.getResponseBuffer(2)) * 0.1;

      modbusdata.pv1voltage = growattInterface.getResponseBuffer(3) * 0.1;
      modbusdata.pv1current = growattInterface.getResponseBuffer(4) * 0.1;
      modbusdata.pv1power = ((growattInterface.getResponseBuffer(5) << 16) | growattInterface.getResponseBuffer(6)) * 0.1;

      modbusdata.pv2voltage = growattInterface.getResponseBuffer(7) * 0.1;
      modbusdata.pv2current = growattInterface.getResponseBuffer(8) * 0.1;
      modbusdata.pv2power = ((growattInterface.getResponseBuffer(9) << 16) | growattInterface.getResponseBuffer(10)) * 0.1;

      // Output
      modbusdata.outputpower = ((growattInterface.getResponseBuffer(35) << 16) | growattInterface.getResponseBuffer(36)) * 0.1;
      modbusdata.gridfrequency = growattInterface.getResponseBuffer(37) * 0.01;
      modbusdata.gridvoltage = growattInterface.getResponseBuffer(38) * 0.1;

      // Energy
      modbusdata.energytoday = ((growattInterface.getResponseBuffer(53) << 16) | growattInterface.getResponseBuffer(54)) * 0.1;
      modbusdata.energytotal = ((growattInterface.getResponseBuffer(55) << 16) | growattInterface.getResponseBuffer(56)) * 0.1;
      modbusdata.totalworktime = ((growattInterface.getResponseBuffer(57) << 16) | growattInterface.getResponseBuffer(58)) * 0.5;

      modbusdata.pv1energytoday = ((growattInterface.getResponseBuffer(59) << 16) | growattInterface.getResponseBuffer(60)) * 0.1;
      modbusdata.pv1energytotal = ((growattInterface.getResponseBuffer(61) << 16) | growattInterface.getResponseBuffer(62)) * 0.1;
      overflow = growattInterface.getResponseBuffer(63);
    }

    if (setcounter == 1) {
      modbusdata.pv2energytoday = ((overflow << 16) | growattInterface.getResponseBuffer(64 - 64)) * 0.1;
      modbusdata.pv2energytotal = ((growattInterface.getResponseBuffer(65 - 64) << 16) | growattInterface.getResponseBuffer(66 - 64)) * 0.1;

      // Temperatures
      modbusdata.tempinverter = growattInterface.getResponseBuffer(93 - 64) * 0.1;
      modbusdata.tempipm = growattInterface.getResponseBuffer(94 - 64) * 0.1;
      modbusdata.tempboost = growattInterface.getResponseBuffer(95 - 64) * 0.1;

      // Diag data
      modbusdata.ipf = growattInterface.getResponseBuffer(100 - 64);
      modbusdata.realoppercent = growattInterface.getResponseBuffer(101 - 64);
      modbusdata.opfullpower = ((growattInterface.getResponseBuffer(102 - 64) << 16) | growattInterface.getResponseBuffer(103 - 64)) * 0.1;
      modbusdata.deratingmode = growattInterface.getResponseBuffer(103 - 64);
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

      modbusdata.faultcode = growattInterface.getResponseBuffer(105 - 64);
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


      modbusdata.faultbitcode = ((growattInterface.getResponseBuffer(105 - 64) << 16) | growattInterface.getResponseBuffer(106 - 64));
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

      modbusdata.warningbitcode = ((growattInterface.getResponseBuffer(110 - 64) << 16) | growattInterface.getResponseBuffer(111 - 64));
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
      //  0x8000
    }

    setcounter++;
    if (setcounter == 2) {
      setcounter = 0;

      // Generate the modbus MQTT message
      sprintf(json, "{", json);
      sprintf(json, "%s \"status\":%d,", json, modbusdata.status);
      sprintf(json, "%s \"solarpower\":%.1f,", json, modbusdata.solarpower);
      sprintf(json, "%s \"pv1voltage\":%.1f,", json, modbusdata.pv1voltage);
      sprintf(json, "%s \"pv1current\":%.1f,", json, modbusdata.pv1current);
      sprintf(json, "%s \"pv1power\":%.1f,", json, modbusdata.pv1power);
      sprintf(json, "%s \"pv2voltage\":%.1f,", json, modbusdata.pv2voltage);
      sprintf(json, "%s \"pv2current\":%.1f,", json, modbusdata.pv2current);
      sprintf(json, "%s \"pv2power\":%.1f,", json, modbusdata.pv2power);

      sprintf(json, "%s \"outputpower\":%.1f,", json, modbusdata.outputpower);
      sprintf(json, "%s \"gridfrequency\":%.2f,", json, modbusdata.gridfrequency);
      sprintf(json, "%s \"gridvoltage\":%.1f,", json, modbusdata.gridvoltage);

      sprintf(json, "%s \"energytoday\":%.1f,", json, modbusdata.energytoday);
      sprintf(json, "%s \"energytotal\":%.1f,", json, modbusdata.energytotal);
      sprintf(json, "%s \"totalworktime\":%.1f,", json, modbusdata.totalworktime);
      sprintf(json, "%s \"pv1energytoday\":%.1f,", json, modbusdata.pv1energytoday);
      sprintf(json, "%s \"pv1energytotal\":%.1f,", json, modbusdata.pv1energytotal);
      sprintf(json, "%s \"pv2energytoday\":%.1f,", json, modbusdata.pv2energytoday);
      sprintf(json, "%s \"pv2energytotal\":%.1f,", json, modbusdata.pv2energytotal);
      sprintf(json, "%s \"opfullpower\":%.1f,", json, modbusdata.opfullpower);

      sprintf(json, "%s \"tempinverter\":%.1f,", json, modbusdata.tempinverter);
      sprintf(json, "%s \"tempipm\":%.1f,", json, modbusdata.tempipm);
      sprintf(json, "%s \"tempboost\":%.1f,", json, modbusdata.tempboost);

      sprintf(json, "%s \"ipf\":%d,", json, modbusdata.ipf);
      sprintf(json, "%s \"realoppercent\":%d,", json, modbusdata.realoppercent);
      sprintf(json, "%s \"deratingmode\":%d,", json, modbusdata.deratingmode);
      sprintf(json, "%s \"faultcode\":%d,", json, modbusdata.faultcode);
      sprintf(json, "%s \"faultbitcode\":%d,", json, modbusdata.faultbitcode);
      sprintf(json, "%s \"warningbitcode\":%d }", json, modbusdata.warningbitcode);
      return result;
    }
    return Continue;
  } else {
    return result;
  }
}
uint8_t growattIF::ReadHoldingRegisters(char* json) {
  uint8_t result;

  ESP.wdtDisable();
  result = growattInterface.readHoldingRegisters(setcounter * 64, 64);
  ESP.wdtEnable(1);

  if (result == growattInterface.ku8MBSuccess)   {
    if (setcounter == 0) {
      modbussettings.enable = growattInterface.getResponseBuffer(0);
      modbussettings.safetyfuncen = growattInterface.getResponseBuffer(1); // Safety Function Enabled
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
      modbussettings.maxoutputactivepp = growattInterface.getResponseBuffer(3); // Inverter M ax output active power percent  0-100: %, 255: not limited
      modbussettings.maxoutputreactivepp = growattInterface.getResponseBuffer(4); // Inverter M ax output reactive power percent  0-100: %, 255: not limited
      modbussettings.maxpower = ((growattInterface.getResponseBuffer(6) << 16) | growattInterface.getResponseBuffer(7)) * 0.1;
      modbussettings.voltnormal = growattInterface.getResponseBuffer(8) * 0.1;
      strncpy(modbussettings.firmware, "      ", 6);
      modbussettings.firmware[0] = growattInterface.getResponseBuffer(9) >> 8;
      modbussettings.firmware[1] = growattInterface.getResponseBuffer(9) & 0xff;
      modbussettings.firmware[2] = growattInterface.getResponseBuffer(10) >> 8;
      modbussettings.firmware[3] = growattInterface.getResponseBuffer(10) & 0xff;
      modbussettings.firmware[4] = growattInterface.getResponseBuffer(11) >> 8;
      modbussettings.firmware[5] = growattInterface.getResponseBuffer(11) & 0xff;

      strncpy(modbussettings.controlfirmware, "      ", 6);
      modbussettings.controlfirmware[0] = growattInterface.getResponseBuffer(12) >> 8;
      modbussettings.controlfirmware[1] = growattInterface.getResponseBuffer(12) & 0xff;
      modbussettings.controlfirmware[2] = growattInterface.getResponseBuffer(13) >> 8;
      modbussettings.controlfirmware[3] = growattInterface.getResponseBuffer(13) & 0xff;
      modbussettings.controlfirmware[4] = growattInterface.getResponseBuffer(14) >> 8;
      modbussettings.controlfirmware[5] = growattInterface.getResponseBuffer(14) & 0xff;

      modbussettings.startvoltage = growattInterface.getResponseBuffer(17) * 0.1;

      strncpy(modbussettings.serial, "          ", 10);
      modbussettings.serial[0] = growattInterface.getResponseBuffer(23) >> 8;
      modbussettings.serial[1] = growattInterface.getResponseBuffer(23) & 0xff;
      modbussettings.serial[2] = growattInterface.getResponseBuffer(24) >> 8;
      modbussettings.serial[3] = growattInterface.getResponseBuffer(24) & 0xff;
      modbussettings.serial[4] = growattInterface.getResponseBuffer(25) >> 8;
      modbussettings.serial[5] = growattInterface.getResponseBuffer(25) & 0xff;
      modbussettings.serial[6] = growattInterface.getResponseBuffer(26) >> 8;
      modbussettings.serial[7] = growattInterface.getResponseBuffer(26) & 0xff;
      modbussettings.serial[8] = growattInterface.getResponseBuffer(27) >> 8;
      modbussettings.serial[9] = growattInterface.getResponseBuffer(27) & 0xff;

      modbussettings.gridvoltlowlimit = growattInterface.getResponseBuffer(52) * 0.1;
      modbussettings.gridvolthighlimit = growattInterface.getResponseBuffer(53) * 0.1;
      modbussettings.gridfreqlowlimit = growattInterface.getResponseBuffer(54) * 0.01;
      modbussettings.gridfreqhighlimit = growattInterface.getResponseBuffer(55) * 0.01;
    }
    if (setcounter == 1) {
      modbussettings.gridvoltlowconnlimit = growattInterface.getResponseBuffer(64 - 64) * 0.1;
      modbussettings.gridvolthighconnlimit = growattInterface.getResponseBuffer(65 - 64) * 0.1;
      modbussettings.gridfreqlowconnlimit = growattInterface.getResponseBuffer(66 - 64) * 0.01;
      modbussettings.gridfreqhighconnlimit = growattInterface.getResponseBuffer(67 - 64) * 0.01;
    }

    setcounter++;
    if (setcounter == 2) {
      setcounter = 0;
      // Generate the modbus MQTT message
      sprintf(json, "{", json);
      sprintf(json, "%s \"enable\":%d,", json, modbussettings.enable);
      sprintf(json, "%s \"safetyfuncen\":%d,", json, modbussettings.safetyfuncen);
      sprintf(json, "%s \"maxoutputactivepp\":%d,", json, modbussettings.maxoutputactivepp);
      sprintf(json, "%s \"maxoutputreactivepp\":%d,", json, modbussettings.maxoutputreactivepp);
      
      sprintf(json, "%s \"maxpower\":%.1f,", json, modbussettings.maxpower);
      sprintf(json, "%s \"voltnormal\":%.1f,", json, modbussettings.voltnormal);
      sprintf(json, "%s \"startvoltage\":%.1f,", json, modbussettings.startvoltage);
      sprintf(json, "%s \"gridvoltlowlimit\":%.1f,", json, modbussettings.gridvoltlowlimit);
      sprintf(json, "%s \"gridvolthighlimit\":%.1f,", json, modbussettings.gridvolthighlimit);
      sprintf(json, "%s \"gridfreqlowlimit\":%.1f,", json, modbussettings.gridfreqlowlimit);
      sprintf(json, "%s \"gridfreqhighlimit\":%.1f,", json, modbussettings.gridfreqhighlimit);
      sprintf(json, "%s \"gridvoltlowconnlimit\":%.1f,", json, modbussettings.gridvoltlowconnlimit);
      sprintf(json, "%s \"gridvolthighconnlimit\":%.1f,", json, modbussettings.gridvolthighconnlimit);
      sprintf(json, "%s \"gridfreqlowconnlimit\":%.1f,", json, modbussettings.gridfreqlowconnlimit);
      sprintf(json, "%s \"gridfreqhighconnlimit\":%.1f,", json, modbussettings.gridfreqhighconnlimit);

      sprintf(json, "%s \"firmware\":\"%s\",", json, modbussettings.firmware);
      sprintf(json, "%s \"controlfirmware\":\"%s\",", json, modbussettings.controlfirmware);
      sprintf(json, "%s \"serial\":\"%s\" }", json, modbussettings.serial);
      return result;
    }
   return Continue;
  } else {
    return result;
  }
}

String growattIF::sendModbusError(uint8_t result) {
  String message = "";
  if (result == growattInterface.ku8MBIllegalFunction) {
    message = "Illegal function";
  }
  if (result == growattInterface.ku8MBIllegalDataAddress) {
    message = "Illegal data address";
  }
  if (result == growattInterface.ku8MBIllegalDataValue) {
    message = "Illegal data value";
  }
  if (result == growattInterface.ku8MBSlaveDeviceFailure) {
    message = "Slave device failure";
  }
  if (result == growattInterface.ku8MBInvalidSlaveID) {
    message = "Invalid slave ID";
  }
  if (result == growattInterface.ku8MBInvalidFunction) {
    message = "Invalid function";
  }
  if (result == growattInterface.ku8MBResponseTimedOut) {
    message = "Response timed out";
  }
  if (result == growattInterface.ku8MBInvalidCRC) {
    message = "Invalid CRC";
  }
  if (message == "") {
    message = result;
  }
  return message;
}
