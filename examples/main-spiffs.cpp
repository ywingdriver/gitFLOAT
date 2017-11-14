#include "Arduino.h"
#include "FS.h"

void write(){
  File f = SPIFFS.open("/accel.txt", "w");

  if (!f) {
    Serial.println("file open for writing failed");
  } else {
    f.println("Accel, Test, File");
    f.print("!");
  }

  f.close();
}

void read(){
  File f = SPIFFS.open("/accel.txt", "r");

  if (!f) {
    Serial.println("file open for reading failed");
  } else {
    Serial.println("----------From File----------");
    String line = f.readStringUntil('!');
    Serial.println(line);
  }

  f.close();
}

void setup() {
  Serial.begin(9600);
  SPIFFS.begin();

  write();
  read();

  Serial.println("done");
}

void loop() {

}
