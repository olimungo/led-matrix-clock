void startWebServer() {
  MDNS.begin("clock");

  server.on("/", menu);

  server.on("/action/clock/display", clockDisplay);
  
  server.on("/action/timer/minute1/oneMore", timerMinute1OneMore);
  server.on("/action/timer/minute1/oneLess", timerMinute1OneLess);
  server.on("/action/timer/minute2/oneMore", timerMinute2OneMore);
  server.on("/action/timer/minute2/oneLess", timerMinute2OneLess);

  server.on("/action/timer/second1/oneMore", timerSecond1OneMore);
  server.on("/action/timer/second1/oneLess", timerSecond1OneLess);
  server.on("/action/timer/second2/oneMore", timerSecond2OneMore);
  server.on("/action/timer/second2/oneLess", timerSecond2OneLess);

  server.on("/action/timer/minutes/fiveMore", timerMinutesFiveMore);
  server.on("/action/timer/minutes/fiveLess", timerMinutesFiveLess);
  
  server.on("/action/timer/start", timerStart);
  server.on("/action/timer/pause", timerPause);
  server.on("/action/timer/reset", timerReset);

  server.on("/action/chrono/start", chronoStart);
  server.on("/action/chrono/pause", chronoPause);
  server.on("/action/chrono/reset", chronoReset);

  server.on("/action/timezone/oneMore", timezoneOneMore);
  server.on("/action/timezone/oneLess", timezoneOneLess);

  server.on("/action/brightness/oneMore", brightnessOneMore);
  server.on("/action/brightness/oneLess", brightnessOneLess);
  
  server.onNotFound(handleNotFound);
  
  server.begin();
}

void menu() {
  String pageHtml = menuHtml;

  pageHtml.replace("%%minute1%%", String(timer.minute1));
  pageHtml.replace("%%minute2%%", String(timer.minute2));
  pageHtml.replace("%%second1%%", String(timer.second1));
  pageHtml.replace("%%second2%%", String(timer.second2));
  pageHtml.replace("%%shift%%", String(timezone.shift));
  pageHtml.replace("%%brightness%%", String(brightness.value));
  
  server.send(200, "text/html", pageHtml);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: QUOD GRATIS ASSERITUR GRATIS NEGATUR");
}

void sendOkReply() {
  server.send(200, "text/html", "OK");
}
