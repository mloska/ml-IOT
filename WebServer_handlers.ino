


File fsUploadFile;


void handle_reset() {
  webServer.sendHeader("Location", "/", true); //Redirect to our html web page
  webServer.send(302, "text/plane", "");
  delay(1000);
  ESP.restart();
}

void handle_loadFormData() {
  DebugSerialPrintln("-handle_loadFormData()");
  if (!webServer.hasArg("param")) {
    webServer.send(400, "text/plain", "400: Invalid Request");
    Serial.println( "Invalid Request 1");
  }
  else {
    char temp2[20];
    webServer.arg("param").toCharArray(temp2, sizeof(temp2));
    if (strcmp (temp2, "ssid") == 0)
      webServer.send ( 200, "text/plain", app_conf.ssid);
    else if (strcmp (temp2, "pass") == 0)
      webServer.send ( 200, "text/plain", app_conf.pass);
    else if (strcmp (temp2, "devName") == 0)
      webServer.send ( 200, "text/plain", app_conf.devName);
    else if (strcmp (temp2, "blynk") == 0)
      webServer.send ( 200, "text/plain", app_conf.blynk);
          else if (strcmp (temp2, "mqttServerAddress") == 0)
      webServer.send ( 200, "text/plain", app_conf.mqttServerAddress);
          else if (strcmp (temp2, "mqttPrefix") == 0)
      webServer.send ( 200, "text/plain", app_conf.mqttPrefix);
    else {
      webServer.send(400, "text/plain", "400: Invalid Request");
      Serial.println( "400: Invalid Request");

    }

  }

}
void handle_set_wifi() {
  DebugSerialPrintln("-handle_set_wifi()");
  if (!webServer.hasArg("ssid") || !webServer.hasArg("pass") ) {
    webServer.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
  }
  else {
    webServer.arg("ssid").toCharArray(app_conf.ssid, sizeof(app_conf.ssid));
    webServer.arg("pass").toCharArray(app_conf.pass, sizeof(app_conf.pass));

    Serial.println("ssid:" + String(app_conf.ssid) + ", pass: " + String(app_conf.pass));
    saveConfig();
  }
  webServer.sendHeader("Location", "/", true); //Redirect to our html web page
  webServer.send(302, "text/plane", "");
}

void handle_set_mqtt() {
  DebugSerialPrintln("-handle_set_mqtt()");
  if (!webServer.hasArg("mqttServerAddress") ) {
    webServer.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
  }
  else {
    webServer.arg("mqttServerAddress").toCharArray(app_conf.mqttServerAddress, sizeof(app_conf.mqttServerAddress));
    webServer.arg("mqttPrefix").toCharArray(app_conf.mqttPrefix, sizeof(app_conf.mqttPrefix));
    saveConfig();
  }
  webServer.sendHeader("Location", "/", true); //Redirect to our html web page
  webServer.send(302, "text/plane", "");
}
void handle_set_devName() {
  DebugSerialPrintln("-handle_set_devName()");
  if (!webServer.hasArg("devName") ) {
    webServer.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
  }
  else {
    webServer.arg("devName").toCharArray(app_conf.devName, sizeof(app_conf.devName));
    saveConfig();
  }
  webServer.sendHeader("Location", "/", true); //Redirect to our html web page
  webServer.send(302, "text/plane", "");
}
void handle_set_blynk() {
  DebugSerialPrintln("-handle_set_blynk()");
  if (!webServer.hasArg("blynk") ) {
    webServer.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
  }
  else {
    webServer.arg("blynk").toCharArray(app_conf.blynk, sizeof(app_conf.blynk));
    // #todo - save
  }
}




String getContentType(String filename) {
  if (webServer.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


bool handle_FileRead(String path) {
  Serial.println("- handle_FileRead(): " + path);
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  Serial.println("content: " + contentType);

  if ( filesystem->exists(path)) {
    Serial.println(F("file found "));
    Serial.println(path);
    File file = filesystem->open(path, "r");
    size_t sent = webServer.streamFile(file, contentType);
    file.close();
    return true;
  }
  Serial.println(F("file NOT found "));
  String str = "";

  Dir dir = filesystem->openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\r\n";
  }
  Serial.println("Lista:");
  Serial.println(str);
  return false;

}



void handleFileUpload() {
  if (webServer.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = webServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = filesystem->open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
    Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
  }
}

void handleFileDelete() {
  if (webServer.args() == 0) {
    return webServer.send(500, "text/plain", "BAD ARGS");
  }
  String path = webServer.arg(0);
  Serial.println("handleFileDelete: " + path);
  if (path == "/") {
    return webServer.send(500, "text/plain", "BAD PATH");
  }
  if (!filesystem->exists(path)) {
    return webServer.send(404, "text/plain", "FileNotFound");
  }
  filesystem->remove(path);
  webServer.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate() {
  if (webServer.args() == 0) {
    return webServer.send(500, "text/plain", "BAD ARGS");
  }
  String path = webServer.arg(0);
  Serial.println("handleFileCreate: " + path);
  if (path == "/") {
    return webServer.send(500, "text/plain", "BAD PATH");
  }
  if (filesystem->exists(path)) {
    return webServer.send(500, "text/plain", "FILE EXISTS");
  }
  File file = filesystem->open(path, "w");
  if (file) {
    file.close();
  } else {
    return webServer.send(500, "text/plain", "CREATE FAILED");
  }
  webServer.send(200, "text/plain", "");
  path = String();
}

void handleFileList() {
  if (!webServer.hasArg("dir")) {
    webServer.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = webServer.arg("dir");
  Serial.println("handleFileList: " + path);
  Dir dir = filesystem->openDir(path);
  path = String();

  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") {
      output += ',';
    }
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    if (entry.name()[0] == '/') {
      output += &(entry.name()[1]);
    } else {
      output += entry.name();
    }
    output += "\"}";
    entry.close();
  }

  output += "]";
  webServer.send(200, "text/json", output);
}




//void handleWiFiConfSetting() {
//  DebugSerialPrintln("- handleWiFiConfSetting()");
//  if (webServer.hasArg("ssid") && webServer.hasArg("psk") && webServer.hasArg("blynk")) {
//    webServer.arg("ssid").toCharArray(app_conf.ssid, sizeof(app_conf.ssid));
//    webServer.arg("psk").toCharArray(app_conf.pass, sizeof(app_conf.pass));
//    webServer.arg("blynk").toCharArray(app_conf.blynk, sizeof(app_conf.blynk));
//
//    if (saveConfig("/config.txt", app_conf)) {
//      webServer.send(200, "text/plain", "Device will be restarted");
//      delay(500);
//      WIFI_init();
//    }
//  }
//}
