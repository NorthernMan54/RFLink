// ************************************* //
// * Arduino Project RFLink-esp        * //
// * https://github.com/couin3/RFLink  * //
// * 2018..2020 Stormteam - Marc RIVES * //
// * More details in RFLink.ino file   * //
// ************************************* //

#ifndef CREDENTIALS_h
#define CREDENTIALS_h

#include "RFLink.h"

#ifndef ESP8266
// local AP
String WIFI_SSID = "67 Bonacres";
String WIFI_PSWD = "brownieg";
#else
#include <ESP8266WiFiMulti.h>
void _AddAccessPoint(ESP8266WiFiMulti *wifiMulti, char *SSID, char *PWD)
{
    Serial.println("Wifi Access : " + String(SSID));
    wifiMulti->addAP(SSID, PWD);
}
void AddAccessPoints(ESP8266WiFiMulti *wifiMulti)
{
    _AddAccessPoint(wifiMulti, "67 Bonacres", "brownieg");
    _AddAccessPoint(wifiMulti, "The_Beach", "brownieg");
}
#endif

// DHCP or Static IP
#define USE_DHCP
#ifndef USE_DHCP
String WIFI_IP = "192.168.1.xxx";
String WIFI_DNS = "192.168.1.xxx";
String WIFI_GATEWAY = "192.168.1.xxx";
String WIFI_SUBNET = "255.255.255.0";
#endif

// MQTT Server
String MQTT_SERVER = "mqtt.local";
String MQTT_PORT = "1883";
// String MQTT_ID = "ESP8266-RFLink_002";   // Set to device name
String MQTT_USER = "xxx";
String MQTT_PSWD = "xxx";

// MQTT Topic will be prepended with device name
String MQTT_TOPIC_OUT = "/msg";
String MQTT_TOPIC_IN = "/cmd";
String MQTT_TOPIC_LWT = "/lwt";

#ifdef CHECK_CACERT
static const char ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIID (...)
-----END CERTIFICATE-----
)EOF";
#endif //CHECK_CACERT

#endif
