#ifndef EASY_SERVER_H
#define EASY_SERVER_H

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

#define SERVER_PORT 80

class EasyServer
{
  public:
    EasyServer();
    ~EasyServer();
    char* wifiInfo();
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
    ESP8266WebServer *server;
  private:
    const char* name;
    const char* password;
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
