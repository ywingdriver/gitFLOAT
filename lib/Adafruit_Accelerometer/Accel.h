#ifndef ACCEL_H
#define ACCEL_H

#include <Wire.h>
#include <SPI.h>

class Accel
{
  public:
    float getAccelData();
    void init();
};

#endif
