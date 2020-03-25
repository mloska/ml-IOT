/*
   Mateusz Loska 2017

   Server App that allows to control the Relay by sending proper HTTP request and external interrupt.

   http://checkip.dyndns.org/


   - Includes FSWebServer - allows to update files to SPIFFS - for file in `\ls -A1`; do curl -F "file=@$PWD/$file" 192.168.10.1/edit; done
*/

// ------------------ D E F I N E S --------------------
//    Change defines when running normal app to avoid using memmory
#define DebugSerialPrintln(a) (Serial.println(a))
#define DebugSerialPrint(a) (Serial.print(a))

// ------------------ L I B R A R I E S-----------------
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleEsp8266.h>                         // https://github.com/blynkkk/blynk-library/releases/tag/v0.6.1 
#include <PubSubClient.h>                               // https://pubsubclient.knolleary.net/api.html
#include <ESP8266mDNS.h>
//#include <OneWire.h> 
#include <ArduinoOTA.h>
#include <FS.h> // file system - SPIFFS
#include <LittleFS.h>
#include "ml-IOT.h"

// ------------------ G L O B A L S --------------------
//network related
      
ESP8266WebServer webServer(80);

char result[1024];                                       // this stores results of string functions (declared globaly to avoid dyn. mem. allocations) #todo - make it more neat
char temp[1024];                                         // #todo - make it more neat

Conf app_conf;
Stat app_status;
// ------------------ S E T U P ----------------------
void setup() {
  SERIAL_init(); 
  SPIFFS_init();
  Config_init();
  WIFI_init();
  WebServer_init();
  MQTT_init();
    IO_init();
  OTA_init();
  
}
// ------------------ L O O O P ----------------------
void loop() {
  // put your main code here, to run repeatedly:
  WIFI_loop();
  OTA_loop();
  WebServer_loop();
  mdns_loop();
  MQTT_Loop();
  IO_loop();
}
