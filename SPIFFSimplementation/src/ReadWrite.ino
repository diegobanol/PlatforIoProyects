#include "FS.h"
#define ROWS 200


void setup() {
  Serial.begin(115200);

  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);

  // this opens the file "f.txt" in read-mode

  File f = SPIFFS.open("/probe.txt", "a"); //r+ open in read and write mode
  f.println("ssid=abc");
  f.println("password=123455secret");
  f.close();
  f = SPIFFS.open("/probe.txt", "r");
  //Array of Strings for the RFID keys
  char *keys [ROWS];
  int i=0;

  if (!f) {
    Serial.println("File doesn't exist yet. Creating it");

    // open the file in write mode
    File f = SPIFFS.open("/probe.txt", "w");

    if (!f) {
      Serial.println("file creation failed");
    }
    // now write two lines in key/value style with  end-of-line characters
    f.println("ssid=abc");
    f.println("password=123455secret");
  } else {
    // we could open the file
    while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('\n');
      keys[i] = (char *) malloc(strlen(line.c_str()));
      strcpy(keys[i], line.c_str());
      Serial.println(strlen(keys[i]));
      i++;
    }
  }
  Serial.println(sizeof(keys)/sizeof(*keys));
  for(i = 0; i < 200; i++ ){
   Serial.println(keys[i]);
  }
  f.close();
}

void loop() {
  // nothing to do for now, this is just a simple test
}
