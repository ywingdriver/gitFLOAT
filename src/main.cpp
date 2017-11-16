#include <Accel.h>
#include <Temp.h>
#include <GPS.h>
#include <EasyServer.h>
#include <LiquidCrystal_I2C.h>
#include <Util.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <stdlib.h>
#include <vector>
#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <sstream>

#define CHECK_FREQUENCY 2

Accel myAccel;
GPS myGPS;
Temp myTemp;
LiquidCrystal_I2C lcd(0x3f,16,2);
ESP8266WebServer server(80);

float gpsLat;
float gpsLon;
float gpsSpeed;
float accelD;
float tempD;

const char* ssid = "William iPhone";
const char* password = "thorincook";

float latNow = 0;
float longNow = 0;
std::vector< float > accelV;
std::vector< float > latV;
std::vector< float > longV;
std::vector< float > tempV;
std::vector< int > timeV;

int loopTracker = 0; // Controls when info is updated

// Forward declarations
void buildWebPage();

void setup() {
  Serial.begin(9600);
  Serial.println("Start setup");
  WiFi.mode(WIFI_STA); // SETS TO STATION MODE!
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  delay(2000);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  myGPS.init();
  myTemp.init();
  myAccel.init();

  lcd.begin(16,2);
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("INIT");

  Serial.println();
  Serial.println("Start data read....");
  Serial.println();

  server.on("/", buildWebPage);
  server.begin();
}

void buildWebPage() {
    // CSS/header
    String html = "<!DOCTYPE html><html><head><title></title><meta charset= 'utf-8' /><meta name= 'viewport' content= 'width=device-width' /><link href='https://fonts.googleapis.com/css?family=Advent+Pro' rel= 'stylesheet' type= 'text/css'><style>html{height:100%}body{height:100%}h2{font-size:90px;font-weight:400;margin:0}svg{position:absolute;width:100%;height:100%}#divTL{position:absolute;top:0;left:0;width:50%;height:50%;background:rgba(255,0,0,0.3)}#divTR{position:absolute;top:0;right:0;width:50%;height:50%;background:rgba(0,255,0,0.3)}#divBL{position:absolute;bottom:0;left:0;width:50%;height:50%;background:rgba(0,0,255,0.3)}#divBR{position:absolute;bottom:0;right:0;width:50%;height:50%}.mytemp{color:#fff;font-family:'Advent Pro';font-weight:400;left:50%;position:absolute;text-align:center;top:50%;transform:translateX(-50%) translateY(-50%);z-index:100;background:rgba(0,0,0,0.3)}.whiteBack{background:linear-gradient(to bottom,rgb(255,255,255),rgb(205,205,205))}.cold{background:linear-gradient(to bottom,#7abcff,#0665e0)}.mediumhot{background:linear-gradient(to bottom,#81ef85,#057003)}.hot{background:linear-gradient(to bottom, #fcdb88, #d32106)}</style> <script src='https://d3js.org/d3.v4.min.js'></script> <script>";

    // Variables
    html += "var titanicLat = ";
    html += gpsLat;
    html += ";";
    html += "var titanicLong = ";
    html += gpsLon;
    html += ";";
    html += "var timeArray = ";
    html += parseVectorInt(timeV);
    html += ";";
    html += "var accelData = [";
    html += parseVector(accelV);
    html += ",";
    html += "[0]";
    html += ",";
    html += "[0]";
    html += "];";
    html += String("var tempData = ") + String(parseVector(tempV));
    html += ";";

    // body
    html += "</script></head><body class='whiteBack'><div id='divTL'></div><div id='divTR'></div><div id='divBL'></div><div id='divBR'></div><div class='mytemp'><h1>Temperature</h1><h2 id='tempText'> 11&nbsp;<small>&deg;C</small></h2></body>";

    // Script
    html += "<script>function initMap(){var t={lat:titanicLat,lng:titanicLong},e=new google.maps.Map(document.getElementById('divBR'),{zoom:9,center:t});new google.maps.Marker({position:t,map:e})}for(API_KEY='AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU',width=window.innerWidth/2,height=window.innerHeight/2,margin=80,svgArray=[],gArray=[],svgName=['Temerature (Celsius)','Accelerometer'],svgColors=['red','blue','yellow','green'],svgArray.push(d3.select('#divTL').append('svg')),svgArray.push(d3.select('#divTR').append('svg')),i=0;2>i;i++)gArray.push(svgArray[i].append('g').attr('transform','translate('+margin/2+','+margin/2+')'));var xTime=d3.scaleTime().rangeRound([0,width-margin]).domain(d3.extent(timeArray,function(t){return console.log(+new Date-(timeArray[timeArray.length-1]-t)),new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-t)))})),yTemp=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(tempData,function(t){return t})),yAccl=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(accelData[0].concat(accelData[1]).concat(accelData[2]),function(t){return t}));lineTemp=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yTemp(t)}),lineAccel=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yAccl(t)});for(var i=0;2>i;++i)gArray[i].append('g').attr('transform','translate(0,'+(height-margin)+')').call(d3.axisBottom(xTime)).select('.domain').remove();gArray[0].append('g').call(d3.axisLeft(yTemp)).append('text').attr('fill',svgColors[3]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[0]),gArray[0].append('path').datum(tempData).attr('fill','none').attr('stroke',svgColors[3]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineTemp),gArray[1].append('g').call(d3.axisLeft(yAccl)).append('text').attr('fill',svgColors[1]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[1]);for(var i=0;3>i;++i)gArray[1].append('path').datum(accelData[i]).attr('fill','none').attr('stroke',svgColors[i]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineAccel);document.getElementById('tempText').innerHTML=''+tempData[tempData.length-1]+'&nbsp;<small>&deg;C</small>';</script> <script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU&callback=initMap'></script> </html>";

    server.send(200, "text/html", html);
}


void loop() {
  accelD = myAccel.getAccelData();
  Serial.println("ACCEL");
  //tempD = myTemp.getTempData();
  tempD = 30.3;
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

  Serial.println("Made it here!2");
  // Update things
  if (loopTracker > CHECK_FREQUENCY){
    accelV.push_back(accelD);
    tempV.push_back(tempD);
    timeV.push_back(millis()/1000);
    loopTracker = 0;
    buildWebPage();
    Serial.println("Made it here!3");
  }
  Serial.println("OISDF");
  ++loopTracker;
  Serial.println("OFDSISDF");
  server.handleClient();
  myGPS.smartDelay(1000);
}
