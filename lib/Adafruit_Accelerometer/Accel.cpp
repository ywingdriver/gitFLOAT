#include <Arduino.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <Accel.h>

/////////////////////////////////////////////////////////////////
/*
  Library created for Accelerometer use
*/
/////////////////////////////////////////////////////////////////

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

int range;
float accelData;

float Accel::getAccelData(){
  // Get information from accelerometer
  lis.read();
  sensors_event_t event;
  lis.getEvent(&event);
  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;

  // Pythagorean representation for accelerometer data
  accelData = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

  return accelData;
}

void Accel::init(){
  if (!lis.begin(0x18)) {
    Serial.println("Couldnt start Accelerometer");
    while (1);
  } else {
    // Set at 8 sensitivity for optimal sensitivity (not too sensitive) when floating
    lis.setRange(LIS3DH_RANGE_8_G);
    range = lis.getRange();
    Serial.println("Accelerometer init successful with " + String(2 << range) +"G sensitivity.");
  }
}
