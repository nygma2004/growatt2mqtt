// Home Assistant MQTT Discovery functions for Growatt Solar Inverter
// This file contains all MQTT discovery configuration messages

#ifndef MQTT_DISCOVERY_H
#define MQTT_DISCOVERY_H

void publishHADiscovery(PubSubClient &mqtt, const char* baseClientID, const char* uniqueClientID, const char* topicRoot, const char* buildversion) {
  char discTopic[128];
  char payload[768];

  // Solar power
  sprintf(discTopic, "homeassistant/sensor/%s_solarpower/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Solar Power\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_solarpower\",\"unit_of_measurement\":\"W\",\"device_class\":\"power\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.solarpower }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Output power
  sprintf(discTopic, "homeassistant/sensor/%s_outputpower/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Output Power\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_outputpower\",\"unit_of_measurement\":\"W\",\"device_class\":\"power\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.outputpower }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Grid voltage
  sprintf(discTopic, "homeassistant/sensor/%s_gridvoltage/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Grid Voltage\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_gridvoltage\",\"unit_of_measurement\":\"V\",\"device_class\":\"voltage\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.gridvoltage }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Grid frequency
  sprintf(discTopic, "homeassistant/sensor/%s_gridfrequency/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Grid Frequency\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_gridfrequency\",\"unit_of_measurement\":\"Hz\",\"device_class\":\"frequency\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.gridfrequency }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Energy today (kWh)
  sprintf(discTopic, "homeassistant/sensor/%s_energytoday/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Energy Today\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_energytoday\",\"unit_of_measurement\":\"kWh\",\"device_class\":\"energy\",\"state_class\":\"total\",\"value_template\":\"{{ value_json.energytoday }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Total energy
  sprintf(discTopic, "homeassistant/sensor/%s_energytotal/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Energy Total\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_energytotal\",\"unit_of_measurement\":\"kWh\",\"device_class\":\"energy\",\"state_class\":\"total_increasing\",\"value_template\":\"{{ value_json.energytotal }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Inverter temperature
  sprintf(discTopic, "homeassistant/sensor/%s_tempinverter/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Inverter Temp\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_tempinverter\",\"unit_of_measurement\":\"°C\",\"device_class\":\"temperature\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.tempinverter }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV1 voltage
  sprintf(discTopic, "homeassistant/sensor/%s_pv1voltage/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV1 Voltage\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv1voltage\",\"unit_of_measurement\":\"V\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.pv1voltage }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV1 current
  sprintf(discTopic, "homeassistant/sensor/%s_pv1current/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV1 Current\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv1current\",\"unit_of_measurement\":\"A\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.pv1current }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV1 power
  sprintf(discTopic, "homeassistant/sensor/%s_pv1power/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV1 Power\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv1power\",\"unit_of_measurement\":\"W\",\"device_class\":\"power\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.pv1power }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Status
  sprintf(discTopic, "homeassistant/sensor/%s_status/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Status\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_status\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.status }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV2 voltage
  sprintf(discTopic, "homeassistant/sensor/%s_pv2voltage/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV2 Voltage\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv2voltage\",\"enabled_by_default\":false,\"unit_of_measurement\":\"V\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.pv2voltage }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV2 current
  sprintf(discTopic, "homeassistant/sensor/%s_pv2current/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV2 Current\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv2current\",\"enabled_by_default\":false,\"unit_of_measurement\":\"A\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.pv2current }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV2 power
  sprintf(discTopic, "homeassistant/sensor/%s_pv2power/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV2 Power\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv2power\",\"enabled_by_default\":false,\"unit_of_measurement\":\"W\",\"device_class\":\"power\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.pv2power }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Total work time
  sprintf(discTopic, "homeassistant/sensor/%s_totalworktime/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Total Work Time\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_totalworktime\",\"unit_of_measurement\":\"s\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.totalworktime }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV1 energy today
  sprintf(discTopic, "homeassistant/sensor/%s_pv1energytoday/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV1 Energy Today\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv1energytoday\",\"unit_of_measurement\":\"kWh\",\"device_class\":\"energy\",\"state_class\":\"total\",\"value_template\":\"{{ value_json.pv1energytoday }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV1 energy total
  sprintf(discTopic, "homeassistant/sensor/%s_pv1energytotal/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV1 Energy Total\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv1energytotal\",\"unit_of_measurement\":\"kWh\",\"device_class\":\"energy\",\"state_class\":\"total_increasing\",\"value_template\":\"{{ value_json.pv1energytotal }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV2 energy today
  sprintf(discTopic, "homeassistant/sensor/%s_pv2energytoday/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV2 Energy Today\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv2energytoday\",\"enabled_by_default\":false,\"unit_of_measurement\":\"kWh\",\"device_class\":\"energy\",\"state_class\":\"total\",\"value_template\":\"{{ value_json.pv2energytoday }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // PV2 energy total
  sprintf(discTopic, "homeassistant/sensor/%s_pv2energytotal/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s PV2 Energy Total\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_pv2energytotal\",\"enabled_by_default\":false,\"unit_of_measurement\":\"kWh\",\"device_class\":\"energy\",\"state_class\":\"total_increasing\",\"value_template\":\"{{ value_json.pv2energytotal }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Output full power
  sprintf(discTopic, "homeassistant/sensor/%s_opfullpower/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Output Full Power\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_opfullpower\",\"enabled_by_default\":false,\"unit_of_measurement\":\"W\",\"device_class\":\"power\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.opfullpower }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // IPM temperature
  sprintf(discTopic, "homeassistant/sensor/%s_tempipm/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s IPM Temp\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_tempipm\",\"unit_of_measurement\":\"°C\",\"device_class\":\"temperature\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.tempipm }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Boost temperature
  sprintf(discTopic, "homeassistant/sensor/%s_tempboost/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Boost Temp\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_tempboost\",\"unit_of_measurement\":\"°C\",\"device_class\":\"temperature\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.tempboost }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Input power factor
  sprintf(discTopic, "homeassistant/sensor/%s_ipf/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Input PF\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_ipf\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.ipf }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Real operation percent
  sprintf(discTopic, "homeassistant/sensor/%s_realoppercent/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Real Op Percent\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_realoppercent\",\"unit_of_measurement\":\"%%\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.realoppercent }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Derating mode
  sprintf(discTopic, "homeassistant/sensor/%s_deratingmode/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Derating Mode\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_deratingmode\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.deratingmode }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Fault code
  sprintf(discTopic, "homeassistant/sensor/%s_faultcode/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Fault Code\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_faultcode\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.faultcode }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Fault bit code
  sprintf(discTopic, "homeassistant/sensor/%s_faultbitcode/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Fault Bit Code\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_faultbitcode\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.faultbitcode }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Warning bit code
  sprintf(discTopic, "homeassistant/sensor/%s_warningbitcode/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Warning Bit Code\",\"state_topic\":\"%s/data\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_warningbitcode\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.warningbitcode }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);
  // Diagnostics: RSSI
  sprintf(discTopic, "homeassistant/sensor/%s_rssi/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s RSSI\",\"state_topic\":\"%s/status\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_rssi\",\"unit_of_measurement\":\"dBm\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.rssi }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Diagnostics: Uptime
  sprintf(discTopic, "homeassistant/sensor/%s_uptime/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Uptime\",\"state_topic\":\"%s/status\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_uptime\",\"unit_of_measurement\":\"s\",\"state_class\":\"measurement\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.uptime }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Diagnostics: SSID
  sprintf(discTopic, "homeassistant/sensor/%s_ssid/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s SSID\",\"state_topic\":\"%s/status\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_ssid\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.ssid }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Diagnostics: IP
  sprintf(discTopic, "homeassistant/sensor/%s_ip/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s IP\",\"state_topic\":\"%s/status\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_ip\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.ip }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Diagnostics: clientid
  sprintf(discTopic, "homeassistant/sensor/%s_clientid/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Client ID\",\"state_topic\":\"%s/status\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_clientid\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.clientid }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Diagnostics: version
  sprintf(discTopic, "homeassistant/sensor/%s_version/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Version\",\"state_topic\":\"%s/status\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_version\",\"entity_category\":\"diagnostic\",\"value_template\":\"{{ value_json.version }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);

  // Max Output Active Power % (from settings)
  sprintf(discTopic, "homeassistant/sensor/%s_maxoutputactivepp/config", uniqueClientID);
  sprintf(payload, "{\"name\":\"%s Max Output Active Power %%\",\"state_topic\":\"%s/settings\",\"availability_topic\":\"%s/connection\",\"unique_id\":\"%s_maxoutputactivepp\",\"unit_of_measurement\":\"%%\",\"state_class\":\"measurement\",\"value_template\":\"{{ value_json.maxoutputactivepp }}\",\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"Growatt Inverter\",\"sw_version\":\"%s\"}}", baseClientID, topicRoot, topicRoot, uniqueClientID, uniqueClientID, baseClientID, buildversion);
  mqtt.publish(discTopic, payload, true);
  delay(50);
}

#endif
