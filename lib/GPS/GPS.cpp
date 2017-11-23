#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <GPS.h>

float lat;
float lon;
float speed;
static const int RXPin = 13, TXPin = 15;
static const int GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

float GPS::getLat(){
	lat = gps.location.lat();
	smartDelay(0);
	return lat;
}

float GPS::getLon(){
	lon = gps.location.lng();
	smartDelay(0);
	return lon;
}

float GPS::getSpeed(){
	speed = gps.speed.kmph();
	smartDelay(0);
	return speed;
}

void GPS::smartDelay(unsigned long ms){
	unsigned long start = millis();
	do
	{
		while (ss.available())
			gps.encode(ss.read());
	} while (millis() - start < ms);
}

void GPS::init(){
	ss.begin(GPSBaud);
	Serial.println("GPS init successful.");
}
