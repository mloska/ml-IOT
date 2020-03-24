void mDNS_init() {
    if (!MDNS.begin(app_conf.devName)) {
        app_status.mDNSConnected = false;
        Serial.println("ERROR: mDNS Responder could not start!");
    }
    else  {
      app_status.mDNSConnected = true;
        Serial.println("mDNS Running,");
         MDNS.addService("http", "tcp", 80);
    }
}

void mdns_loop() {
    MDNS.update();
}
