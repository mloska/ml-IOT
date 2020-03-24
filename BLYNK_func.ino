
// ------------------ D E F I N E S --------------------
//    Change defines when running normal app to avoid using memmory
#define BLYNK_NO_BUILTIN     // Disable built-in analog & digital pin operations
//#define BLYNK_NO_INFO      // Skip device info
#define BLYNK_NO_FLOAT       // Disable float operations
#define BLYNK_PRINT Serial





bool BlynkConnect(char * auth)
{
  Blynk.config(auth);
  Blynk.connect(8442);
  Serial.println(auth);

  DebugSerialPrintln( "" );

  // ... Give ESP 10 seconds to connect to station.
  unsigned long startTime = millis();
  // Wait for connection
  while ( !Blynk.connected() && millis() - startTime < 10000) {
    delay ( 500 );
    DebugSerialPrintln ( "." );

    if ( Blynk.connected())
    {
      return true;
      DebugSerialPrintln("Blynk connected");
    }
    else
    {
      return false;
      DebugSerialPrintln("Blynk NOT connected");
    }
  }
}
