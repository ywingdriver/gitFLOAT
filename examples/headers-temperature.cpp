#include <Temp.h>

Temp myTemp;
float tempRead;

void setup() {
  Serial.begin(9600);

  myTemp.init();

  delay(2000);
}

void loop() {

  tempRead = myTemp.getTempData();
  delay(200);
}
