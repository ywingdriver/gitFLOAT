#ifndef Memory_H
#define Memory_H

#include <Arduino.h>
#include <FS.h>

class Memory
{
  public:
		void append(float accel, float temp, float lat, float lon, float speed);
		void close();
    void read();
    void clear();
		void init();
};

#endif
