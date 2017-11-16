#include <OneWire.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include "EasyServer.h"

// Constructor
EasyServer::EasyServer(int server_port){
    server = new ESP8266WebServer(server_port);
    linksCSS = "";
    css = "";
    linksScript = "";
    scriptHead = "";
    variables = "";
    body = "";
    scriptBody = "";
    linksScriptAsync = "";
    server->on("/", std::bind(&EasyServer::buildWebPage, this));
  }

// Destructor
EasyServer::~EasyServer() {
  if (server)
   delete[] server;
}


// float addLink()
// ---------------------------------------------
// adds a script or stylesheet refrence to the file.
// Takes url="www.yourURL.com/script.js" and type='s' for script, 'c' for stylesheet, 'a' for async.
void EasyServer::addLink(String url, char type) {

    if (type == 'c') { //Stylesheet
      linksCSS += "<link href='" + url;
      linksCSS += "' rel= 'stylesheet' type= 'text/css'>";
    }
    else if (type == 'a') { //Async script at the end of doc (for things like Google Maps)
      linksScriptAsync += "<script async defer src='" + url;
      linksScriptAsync += "'></script>";
    }
    else { //Script
      linksScript += "<script src='" + url;
      linksScript += "'></script>";
    }
  }

// float addCSS()
// ---------------------------------------------
// adds custom CSS to the header of the HTML document.
// Takes string (formatted as CSS stylesheet)
// ex. "h1{ color:blue; }
//      h2{ margin:100;
//          color: green;}"
void EasyServer::addCSS(String cssString) {
    css += cssString;
}

// float addVariable()
// ---------------------------------------------
// adds a custom variable to the header of your file.
// Convienent way to add custom varaibles that will be modified in the later <script> section.
// Takes two Strings: name and value
void EasyServer::addVariable(String name, String value) {
  variables += "var ";
  variables += name;
  variables += " = ";
  variables += value;
  variables += ";";
}

void EasyServer::reset() {
  linksCSS = "";
  css = "";
  linksScript = "";
  scriptHead = "";
  variables = "";
  body = "";
  scriptBody = "";
  linksScriptAsync = "";
}

// float addBody()
// ---------------------------------------------
// Adds the HTML based body of the file inbetween the <body> tags.
// Takes a string of HTML, for example:
// " <h1> Hey there little lassie</h1>
//   <div><h2>How are you my darlin??!?</h2></div> "
void EasyServer::addBody(String htmlString) {
  body += htmlString;
}

// float addHeadScript(), addBodyScript
// ---------------------------------------------
// Adds a section of script in the head or after the body of the document
// Takes a string of Javascript script, for example:
// " var file = file.open("webfile.txt")
//   document.findElement('h2').append('My Text')"
void EasyServer::addHeadScript(String scriptString) {
  scriptHead += scriptString;
}

void EasyServer::addBodyScript(String scriptString) {
  scriptBody += scriptString;
}

// beginServer()
// Begins server process
void EasyServer::beginServer() {
  //buildWebPage();
  server->begin();
}

// handleClient()
// Just a faster way to use the ESP8266WebServer.handleClient() function
void EasyServer::handleClient() {
  server->handleClient();
}

// PRIVATE buildWebpage()
// Takes all of the different compoents of the webpage and builds a thing
void EasyServer::buildWebPage() {

    String html = "<!DOCTYPE html><html><head><title></title><meta charset= 'utf-8' /><meta name= 'viewport' content= 'width=device-width' />";
    html += linksCSS;
    html += "<style>";
    html += css;
    html += "</style>";
    html += linksScript;
    html += "<script>";
    html += variables;
    html += scriptHead;
    html += "</script></head><body>";
    html += body;
    html += "</body><script>";
    html += scriptBody;
    html += "</script>";
    html += linksScriptAsync;
    html += "</html>";

    server->send(200, "text/html", html);
}
