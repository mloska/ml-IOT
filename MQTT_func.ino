
// ------------------ D E F I N E S --------------------
WiFiClient wifiClient;
PubSubClient client(wifiClient);


//mqtt content buffer

long mqttLastConnAttempt = 0;
byte mqtt_send_buffer[64];
char locked_text[] = "LOCKED";
char unlocked_text[] = "UNLOCKED";


// ------------------ I N I T --------------------------
void MQTT_init() {
  client.setServer(app_conf.mqttServerAddress, 1883);
  client.setCallback(mqttSubCallback);

  app_conf.mqttSubTopic[0] = "/setRelay1";
  app_conf.mqttSubTopicCount = 1;

  strcpy(app_conf.mqttStatus_topic, "/state");
  appendSubtopic(app_conf.mqttStatus_topic);
  MQTT_announceLockState();

  Serial.println("Where to listed for state update:");
  Serial.println(app_conf.mqttStatus_topic);

}

void mqttSubCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received topic:");
  Serial.println(topic);
  //DO NOT DELETE this function
  char PDU[length];
  for (unsigned int i = 0; i < (length); i++) {
    PDU[i] = char(payload[i]);
    PDU[i + 1] = '\0';
  }
  Serial.println("MQTT payload received:");
  Serial.println(String(PDU));
  IO_mqttInterrupt(PDU);
}

void MQTT_announceLockState() {
  CharToByte(locked_text, mqtt_send_buffer, 6);
  client.publish(app_conf.mqttStatus_topic, mqtt_send_buffer, 6);
  Serial.println("Announced LOCKED");
}
void MQTT_announceUNLockState() {
  CharToByte(unlocked_text, mqtt_send_buffer, 8);
  client.publish(app_conf.mqttStatus_topic, mqtt_send_buffer, 8);
  Serial.println("Announced UNLOCKED");
  

}
boolean mqttReconnect() {
  client.disconnect();
  if (client.connect(app_conf.devName)) {
    for (int i = 0; i < app_conf.mqttSubTopicCount ; i++ ) {
      appendSubtopic(app_conf.mqttSubTopic[i]);
      Serial.println("Subscribed to MQTT Topic: ");
      Serial.println(app_conf.mqttSubTopic[i]);
      client.subscribe(app_conf.mqttSubTopic[i]);
    }
    Serial.println("Connected to MQTT Server");
    MQTT_announceLockState();
  }
  return client.connected();
}


void MQTT_Loop() {
  if (!client.loop()) {
    long now = millis();
    if (millis() - mqttLastConnAttempt > 5000) {
      mqttLastConnAttempt = now;
      if (mqttReconnect()) {
        Serial.println("MQTT Reconnect Successful.");
      }
      else {
        Serial.println("Still no connection to MQTT Server");
        Serial.println(client.state());
      }
    }
  }
  client.loop();
}

void appendSubtopic(char *inputTopic) {
  char temp_topic[128] = "IOT";
  strcpy(temp_topic, app_conf.mqttPrefix);
  strcat(temp_topic, inputTopic);
  strcpy(inputTopic, temp_topic);
}

void CharToByte(char* chars, byte* bytes, unsigned int count) {
  //DO NOT DELETE this function
  for (unsigned int i = 0; i < count; i++)
    bytes[i] = (byte)chars[i];
}
