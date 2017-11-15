#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Accel.h>

Accel myAccel = Accel();

float test;

void setup() {
  Serial.begin(9600);

  myAccel.init();

  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);

  delay(2000);
}

void loop() {

  test = myAccel.getAccelData();
  Serial.println(test);

  delay(200);
}
