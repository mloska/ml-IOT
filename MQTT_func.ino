
// ------------------ D E F I N E S --------------------
WiFiClient wifiClient;
PubSubClient client(wifiClient);


//mqtt content buffer
char status_topic[128] = "/state";
byte mqtt_send_buffer[64];
long mqttLastConnAttempt = 0;




// ------------------ I N I T --------------------------
void MQTT_init() {
  client.setServer(app_conf.mqttServerAddress, 1883);
  client.setCallback(mqttSubCallback);

  app_conf.mqttSubTopic[0] = "/setRelay1";
  app_conf.mqttSubTopicCount = 1;

}

void mqttSubCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received topic:");
  Serial.println(topic);

  mqttReceivedCallback(topic, payload, length);
}

void mqttReceivedCallback(char* topic, byte* payload, unsigned int length) {
  //DO NOT DELETE this function
  char PDU[length];
  for (unsigned int i = 0; i < (length); i++) {
    PDU[i] = char(payload[i]);
    PDU[i + 1] = '\0';
  }
  //strlcpy(char *destination, const char *source, size_t size);


  Serial.println("MQTT payload received:");
  Serial.println(PDU);

  CharToByte("good", mqtt_send_buffer, 8);
  client.publish(status_topic, mqtt_send_buffer, 8);

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
  char temp_topic[128] = " ";
  strcpy(temp_topic, app_conf.mqttPrefix);
  strcat(temp_topic, inputTopic);
  strcpy(inputTopic, temp_topic);
}

void CharToByte(char* chars, byte* bytes, unsigned int count) {
  //DO NOT DELETE this function
  for (unsigned int i = 0; i < count; i++)
    bytes[i] = (byte)chars[i];
}
