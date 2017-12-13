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
    void parse(int numOfEntries);
    void clear();
    String getAltData();
    String getGpsLatData();
    String getGpsLngData();
    String getGpsSpeedData();
    String getTempData();
    String getLat();
    String getLng();
    String getFakeTimes();
    int getNumEntries();
		void init();
  private:
    String altData;
    String gpsLatData;
    String gpsLngData;
    String gpsSpeedData;
    String tempData;
    String latCur;
    String lngCur;
    String fakeTimes;
    int fakeTime;

};

// Stack Overflow https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index);

#endif
