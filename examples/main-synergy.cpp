#include <Arduino.h>
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS D5

//Temperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

//Accelerometer
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

//LCD
LiquidCrystal_I2C lcd(0x3f,16,2);

char temperatureString[6];
float temperature;
float accelData[3] = {};

void setup() {

  Serial.begin(9600);
  tempSensor.begin();
  Wire.begin();
  lcd.begin(16,2);

  lcd.backlight();

  // Check for Accelerometer
  if (!lis.begin(0x18)) {
    Serial.println("Couldnt start Accelerometer");
    while (1);
  }

  lis.setRange(LIS3DH_RANGE_4_G);
}

float getTemperature() {

  tempSensor.requestTemperatures();

  temperature = tempSensor.getTempCByIndex(0);

  lcd.setCursor(0,0);
  lcd.print(temperature);

  return temperature;
}

float* getAccelData() {

  lis.read();
  sensors_event_t event;
  lis.getEvent(&event);

  float xAccel = event.acceleration.x;
  float yAccel = event.acceleration.y;
  float zAccel = event.acceleration.z;

  accelData[0] = xAccel;
  accelData[1] = yAccel;
  accelData[2] = zAccel;

  lcd.setCursor(6,0);
  lcd.print("X:");
  lcd.setCursor(8,0);
  lcd.print(xAccel);

  lcd.setCursor(0,1);
  lcd.print("Y:");
  lcd.setCursor(2,1);
  lcd.print(yAccel);

  lcd.setCursor(8,1);
  lcd.print("Z: ");
  lcd.setCursor(10,1);
  lcd.print(zAccel);

  return accelData;
}

void loop() {

  lcd.clear();

  float temperature = getTemperature();
  // Serial.println("Temperature:");
  // Serial.println(temperature);

  float* accelInLoop = getAccelData();
  // Serial.println("");
  // for (int i = 0; i < 3 ; i++) {
  //   Serial.println(accelInLoop[i]);
  // }

  delay(1000);
}
