#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <time.h> // time() ctime()
#include <sys/time.h> // struct timeval
#include <coredecls.h> // settimeofday_cb()
#include <Ticker.h>

#include "global.h"
#include "menuHtml.h"
#include "font.h"

void setup(void) {
  Serial.begin(115200);

  startMatrix();

  // To remove default credentials
  // WiFi.disconnect();

  startWifi();
  startWebServer();

  // Specify which function to call back when a reply from NTP is received
  ntpConfigCallback();
  
  ntpGetTime();

  // Refresh time through NTP at a rate specified by REFRESH_RATE_NTP_TIME
  timerRefreshNtpTime.attach(REFRESH_RATE_NTP_TIME, ntpGetTime);

  mx.clear();

  delay(500);

  updateRollReferenceTime(millis());
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  switch (state) {
    case ST_INIT:
      displayInit();
      break;
    case ST_CLOCK:
      displayClock();
      break;
    case ST_TIMER:
      displayTimer();
      break;
    case ST_CHRONO:
      displayChrono();
      break;
  }
}
