#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "global.h"
#include "menuHtml.h"

ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  
  // To remove default credentials
  // WiFi.disconnect();

  startWifi();
  startWebServer();
  
  Serial.println(WiFi.localIP());
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
