
// ------------------ D E F I N E S --------------------
unsigned int timerRepeatConnection_ms = 1000 * 40; //
IPAddress apIP(192, 168, 10, 1);


// ------------------ I N I T --------------------------
void WIFI_init() {
  DebugSerialPrintln(F("WIFI_init - start"));
  WiFi.setAutoConnect (false);      // prevent from automatic reconnections
  WiFi.setAutoReconnect (false);   // prevent from connecting to random WiFis
  app_status.WiFiConnected = false;
  if (app_conf.ssid) {
    DebugSerialPrintln(app_conf.ssid);
    if (!ConnectToWifi(app_conf.ssid, app_conf.pass, 10000)) {
      CreateAP();
    }
    else {
      app_status.WiFiConnected = true;
    }
  }
  else 
    CreateAP();
}


void WIFI_loop() {
  if (app_status.WiFiConnected) {
    if (WiFi.status() != WL_CONNECTED) {
      app_status.WiFiConnected = false;
    }
  }
  else {
    if (millis() - app_status.timerAPstartTime  >= timerRepeatConnection_ms) {
      if (WiFi.softAPgetStationNum() == 0)                // do not try to connect if someone is connected to AP at the moment
      {
        DebugSerialPrintln(F("Trying to reconnect"));
        WIFI_init();
      }
      else {
        DebugSerialPrintln(F("not reconecting - someone connected"));
        app_status.timerAPstartTime = millis();
      }
    }
  }
}

bool ConnectToWifi(String station_ssid, String station_psk, int timeout) {

  DebugSerialPrintln(F("- ConnectToWiFi()"));
  if (WiFi.getMode() != WIFI_STA)
  {
    WiFi.mode(WIFI_STA);
    delay(10);
  }

  // ... Compare file config with sdk config.
  if (WiFi.SSID() != station_ssid || WiFi.psk() != station_psk)
  {
    DebugSerialPrintln(F("WiFi config changed."));

    // ... Try to connect to WiFi station.
    WiFi.begin(station_ssid.c_str(), station_psk.c_str());

    // ... Pritn new SSID
    DebugSerialPrint("new SSID: ");
    DebugSerialPrintln(WiFi.SSID());

    // ... Uncomment this for debugging output.
    
  }
  else
  {
    // ... Begin with sdk config.
    WiFi.begin();
  }

  DebugSerialPrintln(F("Wait for WiFi connection."));

  // ... Give ESP 10 seconds to connect to station.
  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout)
  {
    DebugSerialPrint('.');
    //Serial.print(WiFi.status());
    delay(500);
  }

  DebugSerialPrintln();

  if (WiFi.status() == WL_CONNECTED)
  {

    DebugSerialPrint("IP address: ");
    DebugSerialPrintln(WiFi.localIP());
    mDNS_init();
    return true;
  }
  else {
    return false;
  }
}


void CreateAP() {
  DebugSerialPrintln("CreateAP()");
  WiFi.softAPdisconnect();

  if (WiFi.getMode() != WIFI_AP)
  {
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.mode(WIFI_AP);
    delay(10);
  }
  
  // Go into software AP mode.
  String APname = app_conf.devName;
  APname += String(ESP.getChipId(), HEX);
  APname += ')';
  WiFi.softAP(APname.c_str());
  app_status.timerAPstartTime = millis();
  mDNS_init();
  DebugSerialPrint("IP address: ");
  DebugSerialPrintln(WiFi.softAPIP());

}
