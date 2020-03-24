
// ------------------ D E F I N E S --------------------

// ------------------ I N I T --------------------------
void SERIAL_init(){
  Serial.begin(115200);
  delay(100);
  Serial.println("APP_setup - started");
}
