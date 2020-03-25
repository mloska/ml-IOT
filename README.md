# ml-IOT

ml-IOT - Easily scalable software framework tested on ESP8266 Module developed Home Automation.

Features:
- WIFI connection handling (Access Point is created when there is no opened WIFI connection)
- Handling settings via web interface
- Mqtt Control
- Blynk Control
- OTA Updates


Installation:
- Compile the code and flash it to ESP
- Use Linux OS to upload contents of DATA folder to ESP file system (SPIFFS) 
    Use this command to upload data when CD in the DATA folder " for file in `\ls -A1`; do curl -F "file=@$PWD/$file" 192.168.10.1/edit; "
   
   
