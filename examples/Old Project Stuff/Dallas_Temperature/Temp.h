#ifndef TEMP_H
#define TEMP_H

#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>

class Temp
{
  public:
    float getTempData();
    void init();
};

#endif
