#include <Accel.h>
#include <Temp.h>
#include <GPS.h>
#include <Memory.h>
#include <LiquidCrystal_I2C.h>
#include "lwip/tcp_impl.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EasyServer.h>

/////////////////////////////////////////////////////////////////
/*
  Main file for F.L.O.A.T.
  Contrinbutors (some bad ass motherfuckers):
    Elijah Duckels, William Cook, Patrick McGill
  Outline: Setup contains code that initializes sensors, WIFI, memory, and server.
    The main loop for main sets the state for the program to either sending data
    via WIFI or collecting data from sensors. If a WIFI connection is available,
    then the server code is used, sending data to the web interface. If there
    is no WIFI connection, the program continually collects sensor data and
    writes it to the esp8266 using SPIFFS.
*/
/////////////////////////////////////////////////////////////////

// Forward declarations for functions
void buildWebPage();
void sensorStateLoop();
void wifiStateLoop();

// Initialize created libraries for each sensor, memory, and server
Accel myAccel;
GPS myGPS;
Temp myTemp;
Memory myMemory;
EasyServer easyServer(80);
LiquidCrystal_I2C lcd(0x3f,16,2);

float gpsLat;
float gpsLon;
float gpsSpeed;
float accelD;
float gpsTime;
float tempD = 0;
// Getting 6 values from sensorsS
const int readingSize = 6;
// Data is being read in chunks of 20 and unloading every minute to SPIFFS
const int readingTotal = 21;
int count = 0;
int shown_connection = 0;
// 2D array for holding sensor readings prior to writing to a file
float data[readingTotal][readingSize];
String fileString;

// Specific requirements for connecting to cell phone hotspot
const char* ssid = "Elijah iPhone";
const char* password = "broncos812";

void setup() {
  Serial.begin(9600);

  delay(5000);

  Serial.println("Initializing sensors....");

  myGPS.init();
  myTemp.init();
  myMemory.init();
  myAccel.init();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Clear data file at the beginning of each float
  myMemory.clear();

  lcd.backlight();

  myMemory.close();

  Serial.println();
  Serial.println("Begin sensor reading....");
  easyServer.beginServer();
}

void loop() {
  // Swtich between loop states
  // Check for wifi and if found, unload data to web server, else keep gathering sensor data
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    sensorStateLoop();
    shown_connection = 0;
  }
  else {
    wifiStateLoop();
  }
}

void sensorStateLoop() {
  accelD = myAccel.getAccelData();
  tempD = myTemp.getTempData();
  gpsLat = myGPS.getLat();
  gpsLon = myGPS.getLon();
  gpsSpeed = myGPS.getSpeed();
  gpsTime = myGPS.getTime();

  // Display sensor readings to LCD to show accurate data and sensor loop
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
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);

  // Serial readout
  Serial.print("Accel: "); Serial.println(accelD);
  Serial.print("Temp: "); Serial.println(tempD);
  Serial.print("Lat: "); Serial.println(gpsLat);
  Serial.print("Lon: "); Serial.println(gpsLon);
  Serial.print("Speed: "); Serial.println(gpsSpeed);
  Serial.print("Time: "); Serial.println((gpsTime/100));
  Serial.println();

  float temp[] = {accelD, tempD, gpsLat, gpsLon, gpsSpeed, gpsTime};

  // Creating 2D array from above temp array to later unload to file
  for(int i = 0; i < readingSize; i++){
    data[count][i] = temp[i];
  }

  if (count >= (readingTotal-1)){
    // Unpack data for appending to file
    for(int j = 0; j < (readingTotal-1); j++){
      myMemory.append(data[j][0], data[j][1], data[j][2], data[j][3], data[j][4], data[j][5]);
    }
    myMemory.close();
    count = 0;
  }

  count++;
  easyServer.handleClient();
  // Smart Delay used to keep GPS continually gathering data
  myGPS.smartDelay(500);
}

void wifiStateLoop() {
  if (shown_connection == 0) {
    Serial.println();
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    shown_connection = 1;

    // Display IP to indicate wifi connection
    lcd.setCursor(0,0);
    lcd.print(WiFi.localIP());

    buildWebPage();
    Serial.println("Finished building webpage.");
  }
  easyServer.handleClient();
}

// Compliles variables via easyServer Library
// Adds custon CSS, links, body
void buildWebPage() {
    myMemory.parse(60);
    myMemory.close();
    easyServer.reset();
    Serial.println("Compiling header...");
    easyServer.addLink("https://fonts.googleapis.com/css?family=Advent+Pro", 'c');
    easyServer.addCSS("html{height:100%}body{height:100%}h2{font-size:90px;font-weight:400;margin:0}li{font-size:14px;color:white}h3{font-size:18px;text-align:center;color:white}svg{position:absolute;width:100%;height:100%}#divTL{position:absolute;top:0;left:0;width:50%;height:50%;background:rgba(255,0,0,0.3)}#divTR{position:absolute;top:0;right:0;width:50%;height:50%;background:rgba(0,255,0,0.3)}#divBL{position:absolute;bottom:0;left:0;width:50%;height:50%;background:rgba(0,0,255,0.3)}#divBR{position:absolute;bottom:0;right:0;width:50%;height:50%}#divLastGPS{position:absolute;bottom:120px;right:0;width:250px;background-color:rgba(24,24,24,.5)}.mytemp{color:#fff;font-family:'Advent Pro';font-weight:400;left:50%;position:absolute;text-align:center;top:50%;transform:translateX(-50%) translateY(-50%);z-index:100;background:rgba(0,0,0,0.3)}.whiteBack{background:linear-gradient(to bottom, rgb(255, 255, 255), rgb(205, 205, 205))}");
    easyServer.addLink("https://d3js.org/d3.v4.min.js", 's');

    Serial.println("Compiling varaibles...");
    easyServer.addVariable("titanicLat", myMemory.getLat());
    easyServer.addVariable("titanicLong", myMemory.getLng());
    easyServer.addVariable("timeArray", myMemory.getFakeTimes());

    // Create list
    String accelList = "[";
    accelList += myMemory.getAltData();
    accelList += ",";
    accelList += "[0]";
    accelList += ",";
    accelList += "[0]";
    accelList += "]";
    String gpsList = "[";
    gpsList += myMemory.getGpsLatData();
    gpsList += ",";
    gpsList += myMemory.getGpsLngData();
    gpsList += "]";
    easyServer.addVariable("GPSData", gpsList);
    easyServer.addVariable("accelData", accelList);
    easyServer.addVariable("tempData", myMemory.getTempData());
    easyServer.addVariable("speedData", myMemory.getGpsSpeedData());

    Serial.println("Compiling body...");
    easyServer.addBody("<div id='divTL'></div><div id='divTR'></div><div id='divBL'></div><div id='divBR'></div><div id='divLastGPS'></div><div class='mytemp'><h1>Temperature</h1><h2 id='tempText'> 11&nbsp;<small>&deg;C</small></h2></div>");

    Serial.println("Compiling scripts...");

    easyServer.addLink("https://eventyap.com/float/scriptMain.js", 'b');

    easyServer.addLink("https://maps.googleapis.com/maps/api/js?key=AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU&libraries=visualization&callback=initMap", 'a');
}
