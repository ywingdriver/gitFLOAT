#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setup() {

  Serial.begin(9600);

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");

  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  pinMode(12, OUTPUT);
  pinMode(15, OUTPUT);

  delay(3000);
}

void loop() {
  lis.read();      // get X Y and Z data at once

  // Then print out the raw data
  // if(lis.z <= 500){
  //   Serial.println("Z");
  //   Serial.println(lis.z);
  // }
  // Serial.println("X");
  // Serial.println(lis.x);
  // Serial.println("Y");
  // Serial.println(lis.y);

  /* Or....get a new sensor event, normalized */
  sensors_event_t event;
  lis.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  // if(event.acceleration.z >= 10 || event.acceleration.z <= -10){
  //   digitalWrite(12, HIGH);
  //   Serial.println("ZZZZZZZZZZZZZZZZZZZZZZZZ");
  // } else if (event.acceleration.x >= 10 || event.acceleration.x <= -10){
  //   digitalWrite(15, HIGH);
  //   Serial.println("XXXXXXXXXXXXXXXXXXXXXXXX");
  // } else if(event.acceleration.y >= 10 || event.acceleration.y <= -10){
  //   Serial.println("YYYYYYYYYYYYYYYYYYYYYYYYY");
  // }

  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y);
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
  Serial.println(" m/s^2 ");

  Serial.println();

  delay(200);
}
