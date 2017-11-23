#include <FS.h>
#include <Memory.h>

void Memory::append(float accel, float temp, float lat, float lon, float speed){

  String lineAppend = (String) accel + "," + (String) temp + "," + (String) lat + ","
              + (String) lon + "," + (String) speed;

  File f = SPIFFS.open("/data.txt", "a");

  if (!f) {
    Serial.println("File open for APPENDING failure");
  } else {
    f.println(lineAppend);
  }
}

void Memory::close(){
  File f = SPIFFS.open("/data.txt", "a");

  if (!f) {
    Serial.println("File open for CLOSING failure");
  } else {
    Serial.print("File size: "); Serial.println(f.size());
    f.close();
  }
}

void Memory::read(){
  File f = SPIFFS.open("/data.txt", "r");

  String s = "";

  if (!f) {
    Serial.println("File open for READING failure");
  } else {
    while(f.position() < f.size()){
      s = f.readStringUntil('\n');
      s.trim();
      Serial.println(s);
      delay(500);
    }
  }
  f.close();
}

void Memory::clear(){
  File f = SPIFFS.open("/data.txt", "w");

  if (!f) {
    Serial.println("File open for CLEARING failure");
  } else {
    Serial.println("File clear success");
  }
  f.close();
}

void Memory::init(){
  SPIFFS.begin();

  Serial.println("SPIFFS init successful.");
  //clear();
}
