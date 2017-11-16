#include <Accel.h>
#include <Temp.h>
#include <GPS.h>
#include <LiquidCrystal_I2C.h>

Accel myAccel;
GPS myGPS;
Temp myTemp;
LiquidCrystal_I2C lcd(0x3f,16,2);

float gpsLat;
float gpsLon;
float gpsSpeed;
float accelD;
float tempD;

void setup() {
  Serial.begin(9600);
  Serial.println("Start setup");

  myGPS.init();
  myTemp.init();
  myAccel.init();

  lcd.begin(16,2);
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("INIT");

  delay(3000);

  Serial.println();
  Serial.println("Start data read....");
  Serial.println();
}


void loop() {
  accelD = myAccel.getAccelData();
  tempD = myTemp.getTempData();
  gpsLat = myGPS.getGPSLat();
  gpsLon = myGPS.getGPSLon();
  gpsSpeed = myGPS.getGPSSpeed();

  Serial.print("Accel: "); Serial.println(accelD);
  Serial.print("Temp: "); Serial.println(tempD);
  Serial.print("Lat: "); Serial.println(gpsLat);
  Serial.print("Lon: "); Serial.println(gpsLon);
  Serial.print("Speed: "); Serial.println(gpsSpeed);
  Serial.println();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.setCursor(2,0);
  lcd.print(tempD);

  lcd.setCursor(8,0);
  lcd.print("Acc:");
  lcd.setCursor(12,0);
  lcd.print(accelD);

  lcd.setCursor(0,1);
  lcd.print("GPS");
  lcd.setCursor(3,1);
  lcd.print(gpsLat);

  lcd.setCursor(9,1);
  lcd.print(gpsLon);

  delay(200);
  myGPS.smartDelay(1000);
}
