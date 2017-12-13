#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <GPS.h>

/////////////////////////////////////////////////////////////////
/*
	Library created for GPS use

*/
/////////////////////////////////////////////////////////////////

// GPS variables
float lat;
float lon;
float speed;
float timeGPS;
// Secondary TX,RX pins for esp8266 13 = D7, 15 = D8
static const int RXPin = 13, TXPin = 15;
// Make GPS baud and Serial baud the same
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

float GPS::getTime(){
	timeGPS = gps.time.value();
	smartDelay(0);
	return timeGPS;
}

// Smart Delay used to keep GPS connection during outside processes
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
