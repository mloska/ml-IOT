String APname; // Stores default ESP Name
// ------------------ D E F I N E S --------------------

// ------------------ I N I T --------------------------
void SPIFFS_init() {
  DebugSerialPrintln(F("-SPIFFS_init()"));
  filesystem->begin();
  {
    Dir dir = filesystem->openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }

}


void Config_init() {
  DebugSerialPrintln(F("-Config_init()"));

  APname = "Loska IOT (";
  APname += String(ESP.getChipId(), HEX);
  APname += ')';

  strlcpy(app_conf.devName, APname.c_str(), sizeof(app_conf.devName));
  strlcpy(app_conf.mqttPrefix, String(ESP.getChipId(), HEX).c_str(), sizeof(app_conf.mqttPrefix));


  loadConfigFile("/config.txt", app_conf);
  DebugSerialPrintln(F("Read:"));
}


// LOAD config.txt file
bool loadConfigFile(const char *filename, Conf & configu) {
  DebugSerialPrintln(F("-loadConfigFile()"));

  // open file for reading.
  File configFile = filesystem->open(filename, "r");
  if (!configFile)
  {
    DebugSerialPrintln(F("Failed to open config.txt."));
    return false;
  }
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, configFile);
  if (configFile.size() > 255) {
    DebugSerialPrintln(F("- SIZE TO BIG()"));
  }
  configFile.close();

  if (error)
  {
    Serial.println(F("Failed to read file, using default configuration"));
    return false;
  }

  // Get the root object in the document
  JsonObject root = doc.as<JsonObject>();
  if (root.containsKey("ssid"))
    strlcpy(configu.ssid, root["ssid"], sizeof(configu.ssid));
  if (root.containsKey("pass"))
    strlcpy(configu.pass, root["pass"], sizeof(configu.pass));
  if (root.containsKey("blynk"))
    strlcpy(configu.blynk, root["blynk"], sizeof(configu.blynk));
  if (root.containsKey("devName") && root["devName"][0] != '\0' )
    strlcpy(configu.devName, root["devName"], sizeof(configu.devName));
  if (root.containsKey("mqttServerAddress"))
    strlcpy(configu.mqttServerAddress, root["mqttServerAddress"], sizeof(configu.mqttServerAddress));
  if (root.containsKey("mqttPrefix"))
    strlcpy(configu.mqttPrefix, root["mqttPrefix"], sizeof(configu.mqttPrefix));

  DebugSerialPrintln(F("loadConfigFile() - END result TRUE"));
  return true;
}


bool saveConfig() {
  DebugSerialPrintln(F("-saveConfig()"));
  // Delete existing file, otherwise the configuration is appended to the file
  filesystem->remove("/config.txt");

  // Open file for writing
  File file = filesystem->open("/config.txt", "w");
  if (!file) {
    DebugSerialPrintln(F("Failed to create file"));
    return false;
  }

  // Allocate the document on the stack.
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Make our document contain an object
  JsonObject root = doc.to<JsonObject>();

  // Set the values in the object
  root["ssid"] = app_conf.ssid;
  root["pass"] = app_conf.pass;
  root["mqttServerAddress"] = app_conf.mqttServerAddress;
  if (app_conf.devName[0] != '\0')
    root["devName"] = app_conf.devName;
  else
    root["devName"] = APname.c_str();
  if (app_conf.mqttPrefix[0] != '\0')
    root["mqttPrefix"] = app_conf.mqttPrefix;
  else
    root["mqttPrefix"] = String(ESP.getChipId(), HEX).c_str();


  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    DebugSerialPrintln(F("Failed to write to file"));
    return false;
  }

  // Close the file (File's destructor doesn't close the file)
  file.close();
  DebugSerialPrintln(F("-saveConfig()-OK"));
  return true;
};



// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = filesystem->open(filename, "r");
  if (!file) {
    DebugSerialPrintln(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    DebugSerialPrint((char)file.read());
  }
  DebugSerialPrintln();

  // Close the file (File's destructor doesn't close the file)
  file.close();
}


bool ReadStringStreamFromFile(char * filename, char * tempBuffor, size_t tempBufforSize) {

  File f = filesystem->open(filename, "r");
  if (!f) {
    Serial.println("file open failed");
    return false;
  }
  else
  {
    size_t fileSize = f.size();
    Serial.println(String(fileSize));
    if (fileSize <= tempBufforSize) {

      f.readBytes(tempBuffor, fileSize);
      f.close();
      tempBuffor[fileSize] = '\0'; // when temp is used alreadsy, make sure it will have the ending
      Serial.println(F("Read done"));
      return true;
    } else {
      Serial.println(F("file size is too large"));
      f.close();
      return false;
    }
  }
}


//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}
