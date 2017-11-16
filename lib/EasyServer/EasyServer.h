#ifndef EASYSERVER_H
#define EASYSERVER_H

#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <ESP8266WebServer.h>

class EasyServer
{
  public:
    EasyServer(int);
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
    void reset();
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
