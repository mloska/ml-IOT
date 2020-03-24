#ifndef ml-iot_H
#define ml-iot_H

struct Conf {
  char ssid[24];
  char pass[24];
  char blynk[35];
  char mqttServerAddress[20];
  char devName[30];
  int measurementTime_min;
  char mqttPrefix[20];
  char *mqttSubTopic[16];
  int mqttSubTopicCount;   
} ;

struct Stat {
  bool WiFiCredentialsAvailable;
  bool WiFiConnected;
  bool APConnected;
  long timerAPstartTime;
  bool mDNSConnected;
  
} ;

#endif
