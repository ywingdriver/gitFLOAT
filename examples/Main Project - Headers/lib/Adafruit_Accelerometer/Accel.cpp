#include <Arduino.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <Accel.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

int range;
float accelData;

float Accel::getAccelData(){
  lis.read();

  sensors_event_t event;
  lis.getEvent(&event);

  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;

  accelData = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

  return accelData;
}

void Accel::init(){

  if (!lis.begin(0x18)) {
    Serial.println("Couldnt start Accelerometer");
    while (1);
  } else {
    lis.setRange(LIS3DH_RANGE_8_G);
    range = lis.getRange();
    Serial.println("Accelerometer init successful with " + String(2 << range) +" sensitivity.");
  }
}
