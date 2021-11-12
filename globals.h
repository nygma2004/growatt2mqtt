

char msg[50];
String mqttStat = "";
String message = "";
unsigned long lastTick, uptime, seconds, lastWifiCheck, lastRGB;
int setcounter = 0;
bool ledoff = false;
bool holdingregisters = false;
char newclientid[80];
char buildversion[12]="v1.0.1p2s";
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

  int safetyfuncen, maxoutputactivepp, maxoutputreactivepp;
  float  maxpower, voltnormal, startvoltage, gridvoltlowlimit, gridvolthighlimit, gridfreqlowlimit, gridfreqhighlimit, gridvoltlowconnlimit, gridvolthighconnlimit, gridfreqlowconnlimit, gridfreqhighconnlimit;
  char firmware[6], controlfirmware[6];
  char serial[10];
    
};

struct modbus_holding_registers modbussettings;
