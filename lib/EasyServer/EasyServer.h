#ifndef EASYSERVER_H
#define EASYSERVER_H

#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>

class EasyServer
{
  public:
    EasyServer(int);
    ~EasyServer();
    void addLink(String url, char type);
    void addCSS(String cssString);
    void addVariable(String name, String value);
    void addBody(String htmlString);
    void addHeadScript(String scriptString);
    void addBodyScript(String scriptString);
    void beginServer();
    void updateVariables();
    void handleClient();
    void buildWebPage();
    void reset();
    ESP8266WebServer *server;
  private:
    String linksCSS;
    String linksScript;
    String css;
    String variables;
    String scriptHead;
    String body;
    String scriptBody;
    String linksScriptAsync;
};

#endif
