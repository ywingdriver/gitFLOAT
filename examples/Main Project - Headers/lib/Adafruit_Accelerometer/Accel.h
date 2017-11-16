#ifndef ACCEL_H
#define ACCEL_H

#include <Wire.h>
#include <SPI.h>

class Accel
{
  public:
    int test;
    float getAccelData();
    void init();
};

#endif
