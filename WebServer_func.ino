// ------------------ I N I T --------------------------
bool WebServer_init()
{
  webServer.stop();
  webServer.on ( "/options.js",  []() {
//    ReadStringStreamFromFile("/test.txt", temp, sizeof(temp));
//    replaceTagWithString(result, sizeof(result), temp, "[ssid]", app_conf.ssid);
//    strcpy(temp, result);
//    replaceTagWithString(result, sizeof(result), temp, "[pass]", app_conf.pass);
//    strcpy(temp, result);
    webServer.send(200, "application/javascript", result);
  });
  webServer.on ( "/hotspot-detect.html",  []() {
    if (!handle_FileRead("/"))
      webServer.send(404, "text/plain", "File/page Not Found...");
  } );
  webServer.on("/reset", handle_reset);
  webServer.on("/set_wifi", HTTP_POST, handle_set_wifi);
  webServer.on("/set_mqtt", HTTP_POST, handle_set_mqtt);
  webServer.on("/set_devName", HTTP_POST, handle_set_devName);
  webServer.on("/set_blynk", HTTP_POST, handle_set_blynk);


  webServer.onNotFound([]() {
    if (!handle_FileRead(webServer.uri()))
      webServer.send(404, "text/plain", "Page NOT found");
  });

  webServer.on("/list", HTTP_GET, handleFileList);
  //load editor
  webServer.on("/edit", HTTP_GET, []() {
    if (!handle_FileRead("/edit.htm")) {
      webServer.send(404, "text/plain", "FileNotFound");
    }
  });
  //create file
  webServer.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  webServer.on("/edit", HTTP_DELETE, handleFileDelete);
    webServer.on("/del", HTTP_GET, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  webServer.on("/edit", HTTP_POST, []() {
    webServer.send(200, "text/plain", "");
  }, handleFileUpload);
  //get heap status, analog input value and all GPIO statuses in one json call
  webServer.on("/all", HTTP_GET, []() {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(A0));
    json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
    json += "}";
    webServer.send(200, "text/json", json);
    json = String();
  });
  webServer.begin();
}

// ------------------ L O O P  --------------------------
void WebServer_loop() {
  webServer.handleClient();
}
