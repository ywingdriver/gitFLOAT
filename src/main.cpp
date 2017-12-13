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
  Contrinbutors: Some bad ass motherfuckers a.k.a
    Elijah Duckels, Patrick McGill, William Cook
*/
/////////////////////////////////////////////////////////////////

#define BUILT_WEB_ARRAYS 0

int shown_connection = 0;

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
const int readingSize = 6;
const int readingTotal = 20;
int count = 0;
// 2D array for holding sensor readings prior to writing to a file
float data[readingTotal][readingSize];
String fileString;

// Specific requirements for connecting to the internet
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

  //myMemory.read();
  myMemory.close();

  Serial.println();
  Serial.println("Begin sensor reading....");
  easyServer.beginServer();
}

void loop() {
  // Swtich between loop states
  // Check for wifi and if found, unload data to web server, else keep gathering
  // sensor data
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

  // Display sensor readings to show accurate data and sensor loop
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

  Serial.print("Accel: "); Serial.println(accelD);
  Serial.print("Temp: "); Serial.println(tempD);
  Serial.print("Lat: "); Serial.println(gpsLat);
  Serial.print("Lon: "); Serial.println(gpsLon);
  Serial.print("Speed: "); Serial.println(gpsSpeed);
  Serial.print("Time: "); Serial.println((gpsTime/100));
  Serial.println();

  float temp[] = {accelD, tempD, gpsLat, gpsLon, gpsSpeed};

  // Creating 2D array to later unload to file
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
  }
  easyServer.handleClient();
  delay(100);
}

void buildWebPage() {
    myMemory.parse(70);
    myMemory.close();
    easyServer.reset();
    easyServer.addLink("https://fonts.googleapis.com/css?family=Advent+Pro", 'c');
    easyServer.addCSS("html{height:100%}body{height:100%}h2{font-size:90px;font-weight:400;margin:0}li{font-size:14px;color:white}h3{font-size:18px;text-align:center;color:white}svg{position:absolute;width:100%;height:100%}#divTL{position:absolute;top:0;left:0;width:50%;height:50%;background:rgba(255,0,0,0.3)}#divTR{position:absolute;top:0;right:0;width:50%;height:50%;background:rgba(0,255,0,0.3)}#divBL{position:absolute;bottom:0;left:0;width:50%;height:50%;background:rgba(0,0,255,0.3)}#divBR{position:absolute;bottom:0;right:0;width:50%;height:50%}#divLastGPS{position:absolute;bottom:120px;right:0;width:250px;background-color:rgba(24,24,24,.5)}.mytemp{color:#fff;font-family:'Advent Pro';font-weight:400;left:50%;position:absolute;text-align:center;top:50%;transform:translateX(-50%) translateY(-50%);z-index:100;background:rgba(0,0,0,0.3)}.whiteBack{background:linear-gradient(to bottom, rgb(255, 255, 255), rgb(205, 205, 205))}");
    easyServer.addLink("https://d3js.org/d3.v4.min.js", 's');
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

    easyServer.addBody("<div id='divTL'></div><div id='divTR'></div><div id='divBL'></div><div id='divBR'></div><div id='divLastGPS'></div><div class='mytemp'><h1>Temperature</h1><h2 id='tempText'> 11&nbsp;<small>&deg;C</small></h2></div>");
    easyServer.addBodyScript("function initMap(){var t={lat:titanicLat,lng:titanicLong};map=new google.maps.Map(document.getElementById('divBR'),{zoom:9,center:t});new google.maps.Marker({position:t,map:map});heatmap=new google.maps.visualization.HeatmapLayer({data:processGPSData(),map:map})}function processGPSData(){myPoints=[];for(var t=0;t<GPSData[0].length/2;++t)myPoints.push(new google.maps.LatLng(GPSData[0][t],GPSData[1][t]));return console.log(myPoints),myPoints}API_KEY='AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU';var map;for(width=window.innerWidth/2,height=window.innerHeight/2,margin=80,svgArray=[],gArray=[],svgName=['Temerature (Celsius)','Accelerometer','Speed (mph)'],svgColors=['red','blue','yellow','green'],svgArray.push(d3.select('#divTL').append('svg')),svgArray.push(d3.select('#divTR').append('svg')),svgArray.push(d3.select('#divBL').append('svg')),i=0;i<3;i++)gArray.push(svgArray[i].append('g').attr('transform','translate('+margin/2+','+margin/2+')'));var xTime=d3.scaleTime().rangeRound([0,width-2*margin]).domain(d3.extent(timeArray,function(t){return console.log(+new Date-(timeArray[timeArray.length-1]-t)),new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-t)))})),yTemp=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(tempData,function(t){return t})),yAccl=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(accelData[0].concat(accelData[1]).concat(accelData[2]),function(t){return t})),ySpeed=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(speedData,function(t){return t}));lineTemp=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yTemp(t)}),lineAccel=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yAccl(t)}),lineSpeed=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return ySpeed(t)});for(var i=0;i<3;++i)gArray[i].append('g').attr('transform','translate(0,'+(height-margin)+')').call(d3.axisBottom(xTime)).select('.domain').remove();gArray[0].append('g').call(d3.axisLeft(yTemp)).append('text').attr('fill',svgColors[3]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[0]),gArray[0].append('path').datum(tempData).attr('fill','none').attr('stroke',svgColors[3]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineTemp),gArray[1].append('g').call(d3.axisLeft(yAccl)).append('text').attr('fill',svgColors[1]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[1]);for(var i=0;i<3;++i)gArray[1].append('path').datum(accelData[i]).attr('fill','none').attr('stroke',svgColors[i]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineAccel);gArray[2].append('g').call(d3.axisLeft(ySpeed)).append('text').attr('fill',svgColors[3]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[3]),gArray[2].append('path').datum(speedData).attr('fill','none').attr('stroke',svgColors[2]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineSpeed),document.getElementById('tempText').innerHTML=tempData[tempData.length-1]+'&nbsp;<small>&deg;C</small>',htmlList='<h3>Last 10 GPS readings</h3><ul>',GPSData[0].reverse(),GPSData[1].reverse();for(var i=0;i<10;++i)htmlList+='<li><b>Lat:</b> '+GPSData[0][i].toFixed(3)+'  <b>Lng:</b> '+GPSData[1][i].toFixed(3)+'</li>';document.getElementById('divLastGPS').innerHTML=htmlList;");

    easyServer.addLink("https://maps.googleapis.com/maps/api/js?key=AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU&libraries=visualization&callback=initMap", 'a');

}
