#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <time.h> // time() ctime()
#include <sys/time.h> // struct timeval
#include <coredecls.h> // settimeofday_cb()

#include "global.h"
#include "menuHtml.h"

ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  
  // To remove default credentials
  // WiFi.disconnect();

  startWifi();
  startWebServer();

  ntpConfigCallback();
  ntpGetTime();
  
  Serial.println(WiFi.localIP());
}

void loop(void) {
  static uint32_t aFewMomentAgo = 0;
  uint32_t nowMillis = millis();
  
  server.handleClient();
  MDNS.update();

  if (isTimeSet && nowMillis - aFewMomentAgo > 1000) {
    time_t now = time(nullptr);
    
    Serial.print(ctime(&now));

    aFewMomentAgo = nowMillis;
  }
}
