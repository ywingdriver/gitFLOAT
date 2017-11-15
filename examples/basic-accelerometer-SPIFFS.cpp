#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Accel.h>
#include <FS.h>

Accel myAccel = Accel();

float test;

void append(String msg){
  File f = SPIFFS.open("/accel.txt", "a");

  if (!f) {
    Serial.println("file open for appending failure");
  } else {
    f.println(msg);
    Serial.println(msg);
    delay(1000);
  }
  delay(1000);
}

void close(){
  File f = SPIFFS.open("/accel.txt", "a");

  if (!f) {
    Serial.println("file open for closing failure");
  } else {
    f.println("!");
    f.close();
  }
  delay(1000);
}

void read(){
  File f = SPIFFS.open("/accel.txt", "r");

  if (!f) {
    Serial.println("file open for reading failed");
  } else {
    Serial.println("----------From File----------");
    String line = f.readStringUntil('!');
    Serial.println(line);
    delay(1000);
  }
  f.close();
}

void clear(){
  File f = SPIFFS.open("/accel.txt", "w");

  if (!f) {
    Serial.println("file open failure");
  } else {
    Serial.println("file open success");
  }
  delay(1000);
}

void setup() {
  delay(8000);
  Serial.begin(9600);
  SPIFFS.begin();

  //clear();
  myAccel.init();

  pinMode(15, OUTPUT);

  delay(2000);
}

void loop() {

  // for(int i = 0; i < 5; i++){
  //   test = myAccel.getAccelData();
  //   append(String(test));
  //
  //   delay(200);
  // }
  //
  // close();

  //read();

  // bool removed = SPIFFS.remove("/accel.txt");
  // Serial.println("File removed: " + String(removed));

  Serial.println("Done");
  digitalWrite(15, HIGH);
  delay(1200000);
}
