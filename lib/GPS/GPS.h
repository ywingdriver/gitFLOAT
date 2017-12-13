#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class GPS
{
  public:
		float getLat();
		float getLon();
		float getSpeed();
    float getTime();
		void smartDelay(unsigned long ms);
		void init();
};

#endif
