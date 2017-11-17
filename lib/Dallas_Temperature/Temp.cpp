#include <Wire.h>
#include <OneWire.h>
#include <Temp.h>

#define ONE_WIRE_BUS D5

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp(&oneWire);

float tempData;

float Temp::getTempData(){
  temp.requestTemperatures();
  tempData = temp.getTempCByIndex(0);
  temp.begin();
  return tempData;
}

void Temp::init(){
  temp.begin();
  Wire.begin();
  Serial.println("Temperature init successful.");
}
