void OTA_init() {
  Serial.println("OTA_init()");
    ArduinoOTA.setHostname("test");
    ArduinoOTA.onStart([]() {
        Serial.println("Arduino OTA InProgress...");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("Arduino OTA Ended");
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.println("Arduino OTA Error..restarting...");
        (void)error;
        ESP.restart();
    });
    ArduinoOTA.begin();
    Serial.println("OTA Available");
}

void OTA_loop() {
    ArduinoOTA.handle();
}
