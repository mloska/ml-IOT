
// ------------------ D E F I N E S --------------------

void ICACHE_RAM_ATTR handleInterrupt();

unsigned long timer_relay;
bool relay_state = false;
bool interrupt_state = false;




// Pin Settings
int relay_pin = D1;
int interruptPin = D2; // Pin where button is connected ( pull-up implemented, so interrupt appears when connected to GND)

// ------------------ I N I T --------------------------
void IO_init() {
  pinMode(relay_pin, OUTPUT); //relay output
  pinMode(interruptPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

}



void handleInterrupt() {
  /*
     Below there is a delayMicroseconds which is better then regular delay since it blocks interrupts - with delay the code was hanging
     This part is used to debounce the intput. Its the best implementation, thanks to the delay we can protect the code from running when
     user releses the button.
  */
  delayMicroseconds(40000); //
  if (digitalRead(interruptPin) == 0)
    interrupt_state = true;

}
void IO_openGate(long timeToOpen) {
  MQTT_announceUNLockState();
  timer_relay = millis() + timeToOpen;
  relay_state = true;
  digitalWrite(relay_pin, HIGH);
}

void IO_closeGate() {
  Serial.println("Relay switched off");
  relay_state = false;
  digitalWrite(relay_pin, LOW);
  MQTT_announceLockState();
}
void IO_mqttInterrupt(String PDU) {
  if (PDU == "UNLOCKED")
    IO_openGate(4000);
  else
    MQTT_announceLockState();

}

void IO_loop() {
  // DO NOT DELETE this function
  if (interrupt_state) {
    IO_openGate(10000);
    Serial.println("Relay switched on due to interrupt");
    interrupt_state = false;
  }
  if (relay_state) {
    if (timer_relay < millis()) {
      IO_closeGate();
      Serial.println("Relay switched off after timeout");
    }
  }
}
