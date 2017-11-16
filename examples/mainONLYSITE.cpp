#include <Arduino.h>
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <EasyServer.h>
#include <Util.h>

const char* ssid = "William iPhone";
const char* password = "thorincook";
EasyServer easyServer(80);

std::vector< float > accelXV;
std::vector< float > accelYV;
std::vector< float > accelZV;
std::vector< float > tempV;
std::vector< int > timeV;
int loopTracker = 0; // Controls when info is updated

void buildWebPage();
String parseVector(std::vector<float> v);
String parseVectorInt(std::vector<int> v);

void setup() {

  Serial.begin(9600);
  Serial.println("SDFSDFSDF");
  WiFi.mode(WIFI_STA); // SETS TO STATION MODE!
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  srand(time(NULL));
  easyServer.beginServer();
}

// buildWebpage()
// Takes the values in the 5 global vectors and makes them into a webpage.
void buildWebPage() {
    Serial.println("madeithere254")
    easyServer.reset();
    easyServer.addLink("https://fonts.googleapis.com/css?family=Advent+Pro", 'c');
    easyServer.addCSS("html{height:100%}body{height:100%}h2{font-size:90px;font-weight:400;margin:0}svg{position:absolute;width:100%;height:100%}#divTL{position:absolute;top:0;left:0;width:50%;height:50%;background:rgba(255,0,0,0.3)}#divTR{position:absolute;top:0;right:0;width:50%;height:50%;background:rgba(0,255,0,0.3)}#divBL{position:absolute;bottom:0;left:0;width:50%;height:50%;background:rgba(0,0,255,0.3)}#divBR{position:absolute;bottom:0;right:0;width:50%;height:50%}.mytemp{color:#fff;font-family:'Advent Pro';font-weight:400;left:50%;position:absolute;text-align:center;top:50%;transform:translateX(-50%) translateY(-50%);z-index:100;background:rgba(0,0,0,0.3)}.whiteBack{background:linear-gradient(to bottom,rgb(255,255,255),rgb(205,205,205))}.cold{background:linear-gradient(to bottom,#7abcff,#0665e0)}.mediumhot{background:linear-gradient(to bottom,#81ef85,#057003)}.hot{background:linear-gradient(to bottom, #fcdb88, #d32106)}");
    easyServer.addLink("https://d3js.org/d3.v4.min.js", 's');
    easyServer.addVariable("titanicLat", "47.168900189313");
    easyServer.addVariable("titanicLong", "-109.63099896919");
    easyServer.addVariable("timeArray", parseVectorInt(timeV));

    // Create list
    String varList = "[";
    varList += parseVector(accelXV);
    varList += ",";
    varList += parseVector(accelYV);
    varList += ",";
    varList += parseVector(accelZV);
    varList += "]";
    easyServer.addVariable("accelData", varList);
    easyServer.addVariable("tempData", parseVector(tempV));
    easyServer.addBody("<div id='divTL'></div><div id='divTR'></div><div id='divBL'></div><div id='divBR'></div><div class='mytemp'><h1>Temperature</h1><h2 id='tempText'> 11&nbsp;<small>&deg;C</small></h2>");
    easyServer.addBodyScript("function initMap(){var t={lat:titanicLat,lng:titanicLong},e=new google.maps.Map(document.getElementById('divBR'),{zoom:9,center:t});new google.maps.Marker({position:t,map:e})}for(API_KEY='AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU',width=window.innerWidth/2,height=window.innerHeight/2,margin=80,svgArray=[],gArray=[],svgName=['Temerature (Celsius)','Accelerometer'],svgColors=['red','blue','yellow','green'],svgArray.push(d3.select('#divTL').append('svg')),svgArray.push(d3.select('#divTR').append('svg')),i=0;2>i;i++)gArray.push(svgArray[i].append('g').attr('transform','translate('+margin/2+','+margin/2+')'));var xTime=d3.scaleTime().rangeRound([0,width-margin]).domain(d3.extent(timeArray,function(t){return console.log(+new Date-(timeArray[timeArray.length-1]-t)),new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-t)))})),yTemp=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(tempData,function(t){return t})),yAccl=d3.scaleLinear().rangeRound([height-margin,0]).domain(d3.extent(accelData[0].concat(accelData[1]).concat(accelData[2]),function(t){return t}));lineTemp=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yTemp(t)}),lineAccel=d3.line().x(function(t,e){return xTime(new Date(1e3*(+new Date-(timeArray[timeArray.length-1]-timeArray[e]))))}).y(function(t){return yAccl(t)});for(var i=0;2>i;++i)gArray[i].append('g').attr('transform','translate(0,'+(height-margin)+')').call(d3.axisBottom(xTime)).select('.domain').remove();gArray[0].append('g').call(d3.axisLeft(yTemp)).append('text').attr('fill',svgColors[3]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[0]),gArray[0].append('path').datum(tempData).attr('fill','none').attr('stroke',svgColors[3]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineTemp),gArray[1].append('g').call(d3.axisLeft(yAccl)).append('text').attr('fill',svgColors[1]).attr('transform','rotate(-90)').attr('y',6).attr('dy','0.71em').attr('text-anchor','end').attr('font-size',15).text(svgName[1]);for(var i=0;3>i;++i)gArray[1].append('path').datum(accelData[i]).attr('fill','none').attr('stroke',svgColors[i]).attr('stroke-linejoin','round').attr('stroke-linecap','round').attr('stroke-width',2.5).attr('d',lineAccel);document.getElementById('tempText').innerHTML=''+tempData[tempData.length-1]+'&nbsp;<small>&deg;C</small>';");
    easyServer.addLink("https://maps.googleapis.com/maps/api/js?key=AIzaSyCk-Qr3LlPgeWbOsceNpKBq6DE1pdA_AhU&callback=initMap", 'a');

}

void loop() {

  // In the actual program, getTemerature and getAccelData will append to these vectors.
  if (loopTracker > 10){
    accelXV.push_back((float)(rand() % 4 + 1));
    accelYV.push_back((float)(rand() % 4 + 1));
    accelZV.push_back((float)(rand() % 4 + 1));
    tempV.push_back((float)(rand() % 51));
    timeV.push_back(millis()/1000);
    loopTracker = 0;
    buildWebPage();
  }

  easyServer.handleClient();
  ++loopTracker;
  delay(100);
}
