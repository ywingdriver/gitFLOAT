#include <Accel.h>
#include <Temp.h>
#include <GPS.h>
#include <Memory.h>
#include <LiquidCrystal_I2C.h>
#include <Util.h>
#include <vector>
#include "lwip/tcp_impl.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define CHECK_FREQUENCY 120

void buildWebPage();

void tcpCleanup()
{
  while(tcp_tw_pcbs!=NULL)
  {
    tcp_abort(tcp_tw_pcbs);
  }
}

Accel myAccel;
GPS myGPS;
Temp myTemp;
Memory myMemory;
ESP8266WebServer server(80);
//LiquidCrystal_I2C lcd(0x3f,16,2);

float gpsLat;
float gpsLon;
float gpsSpeed;
float accelD;
float tempD = 0;
std::vector< float > accelV;
std::vector< float > latV;
std::vector< float > lonV;
std::vector< float > tempV;
std::vector< int > timeV;
int loopTracker = 0;
const int readingSize = 5;
const int readingTotal = 120;
int count = 0;
float data[readingTotal][readingSize];


//const char* ssid = "Go Griz";
//const char* password = "Kenpachi";
const char* ssid = "William iPhone";
 const char* password = "thorincook";

void setup() {
  Serial.begin(9600);

  // WiFi.mode(WIFI_STA); // SETS TO STATION MODE!
  // WiFi.begin(ssid, password);
  //
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  delay(5000);

  Serial.println("Initializing sensors....");

  myGPS.init();
  myTemp.init();
  myMemory.init();
  myAccel.init();

  tempD = myTemp.getTempData();

  //****************************************
  // Hey Will this is the thing I was metnioning in class, as long as you leave
  // myMemory.clear() commented, the data should remain on the 8266. You will need to
  // comment out the myMemory.close() and myMemory.read() or else it'll
  // print out the entire file to Serial.
  // myMemory.clear();
  //****************************************
  myMemory.close();
  delay(1000);
  myMemory.read();

  // server.on("/", buildWebPage);
  // server.begin();

  Serial.println();
  Serial.println("Begin sensor reading....");
}

void loop() {
  // Check if wifi is connected
  accelD = myAccel.getAccelData();
  //tempD = myTemp.getTempData();
  gpsLat = myGPS.getLat();
  gpsLon = myGPS.getLon();
  gpsSpeed = myGPS.getSpeed();

  Serial.print("Accel: "); Serial.println(accelD);
  Serial.print("Temp: "); Serial.println(tempD);
  Serial.print("Lat: "); Serial.println(gpsLat);
  Serial.print("Lon: "); Serial.println(gpsLon);
  Serial.print("Speed: "); Serial.println(gpsSpeed);
  Serial.println();

  float temp[] = {accelD, tempD, gpsLat, gpsLon, gpsSpeed};

  for(int i = 0; i < readingSize; i++){
    data[count][i] = temp[i];
  }

  // tcpCleanup();

  if (count >= (readingTotal-1)){
    //tempD = myTemp.getTempData();
    accelV.push_back(accelD);
    tempV.push_back(tempD);
    latV.push_back(gpsLat);
    lonV.push_back(gpsLon);
    timeV.push_back(millis()/1000);
    loopTracker = 0;

    Serial.println("------------------- To File -------------------");
    for(int j = 0; j < (readingTotal-1); j++){
      myMemory.append(data[j][0], data[j][1], data[j][2], data[j][3], data[j][4]);
    }
    myMemory.close();

    count = 0;

    if (static_cast<int>(timeV.size()) > 400) {
      accelV.erase(accelV.begin() + 1);
      tempV.erase(tempV.begin() + 1);
      latV.erase(latV.begin() + 1);
      lonV.erase(lonV.begin() + 1);
      timeV.erase(timeV.begin() + 1);
    }
  }

  loopTracker++;
  count++;
  // server.handleClient();
  myGPS.smartDelay(500);
}

void buildWebPage() {
    // CSS/header
    String html = "<!DOCTYPE html><html><head><title></title><meta charset= 'utf-8' /><meta name= 'viewport' content= 'width=device-width' /><link href='https://fonts.googleapis.com/css?family=Advent+Pro' rel= 'stylesheet' type= 'text/css'><style>li{font-size:20px;color:white;}h3{font-size:30px;text-align:center;color:white;}html{height:100%}body{height:100%}h2{font-size:90px;font-weight:400;margin:0}svg{position:absolute;width:100%;height:100%}#divTL{position:absolute;top:0;left:0;width:50%;height:50%;background:rgba(255,0,0,0.3)}#divTR{position:absolute;top:0;right:0;width:50%;height:50%;background:rgba(0,255,0,0.3)}#divBL{position:absolute;bottom:0;left:0;width:50%;height:50%;background:rgba(0,0,255,0.3)}#divBR{position:absolute;bottom:0;right:0;width:50%;height:50%}.mytemp{color:#fff;font-family:'Advent Pro';font-weight:400;left:50%;position:absolute;text-align:center;top:50%;transform:translateX(-50%) translateY(-50%);z-index:100;background:rgba(0,0,0,0.3)}.whiteBack{background:linear-gradient(to bottom,rgb(255,255,255),rgb(205,205,205))}.cold{background:linear-gradient(to bottom,#7abcff,#0665e0)}.mediumhot{background:linear-gradient(to bottom,#81ef85,#057003)}.hot{background:linear-gradient(to bottom, #fcdb88, #d32106)}</style> <script src='https://d3js.org/d3.v4.min.js'></script> <script>";

    // Variables
    html += "var GPSData = [";
    html += parseVector(latV);
    html += ",";
    html += parseVector(lonV);
    html += "];";
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
    html += "<script>function initMap(){var t={lat:titanicLat,lng:titanicLong},e=new google.maps.Map(document.getElementById('divBR'),{zoom:9,center:t});new google.maps.Marker({position:t,map:e})}for(API_KEY='AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU',width=window.innerWidth/2,height=window.innerHeight/2,margin=80,svgArray=[],gArray=[],svgName=['Temerature (Celsius)','Accelerometer'],svgColors=['red','blue','yellow','green'],svgArray.push(d3.select('#divTL').append('svg')),svgArray.push(d3.select('#divTR').append('svg')),i=0;2>i;i++)gArray.push(svgArray[i].append('g').attr('transform','translate('+margin/2+','+margin/2+')'));var xTime=d3.scaleTime().rangeRound([0,width-margin]).domain(d3.extent(timeArray,function(t){return console.log(+new Date-(timeArray[timeArray.length-1]-t)),new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-t)))})),yTemp=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(tempData,function(t){return t})),yAccl=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(accelData[0].concat(accelData[1]).concat(accelData[2]),function(t){return t}));lineTemp=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yTemp(t)}),lineAccel=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yAccl(t)});for(var i=0;2>i;++i)gArray[i].append('g').attr('transform','translate(0,'+(height-margin)+')').call(d3.axisBottom(xTime)).select('.domain').remove();gArray[0].append('g').call(d3.axisLeft(yTemp)).append('text').attr('fill',svgColors[3]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[0]),gArray[0].append('path').datum(tempData).attr('fill','none').attr('stroke',svgColors[3]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineTemp),gArray[1].append('g').call(d3.axisLeft(yAccl)).append('text').attr('fill',svgColors[1]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[1]);for(var i=0;3>i;++i)gArray[1].append('path').datum(accelData[i]).attr('fill','none').attr('stroke',svgColors[i]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineAccel);document.getElementById('tempText').innerHTML=''+tempData[tempData.length-1]+'&nbsp;<small>&deg;C</small>',htmlList='<h3>Last 10 GPS readings</h3><ul>',GPSData[0].reverse(),GPSData[1].reverse();for(var i=0;10>i;++i)htmlList+='<li>Lat: '+GPSData[0][i]+' Long: '+GPSData[1][i]+'</li>';document.getElementById('divBL').innerHTML=htmlList;</script> <script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU&callback=initMap'></script> </html>";
    server.send(200, "text/html", html);
}

// lcd.setCursor(0,0);
// lcd.print("T:");
// lcd.setCursor(2,0);
// lcd.print(tempD);
//
// lcd.setCursor(8,0);
// lcd.print("Acc:");
// lcd.setCursor(12,0);
// lcd.print(accelD);
//
// lcd.setCursor(0,1);
// lcd.print("GPS");
// lcd.setCursor(3,1);
// lcd.print(gpsLat);
//
// lcd.setCursor(9,1);
// lcd.print(gpsLon);
// lcd.begin(16,2);
// lcd.backlight();
// lcd.setCursor(0,0);
// lcd.print("INIT");
