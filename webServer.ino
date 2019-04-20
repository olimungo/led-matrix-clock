void startWebServer() {
  MDNS.begin("clock");

  server.on("/", menu);
  
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
  
  server.onNotFound(handleNotFound);
  
  server.begin();
}

void menu() {
  String result = html;

  result.replace("%%minute1%%", String(timer.minute1));
  result.replace("%%minute2%%", String(timer.minute2));
  result.replace("%%second1%%", String(timer.second1));
  result.replace("%%second2%%", String(timer.second2));
  result.replace("%%shift%%", String(timezone.shift));
  
  server.send(200, "text/html", result);
}

void handleNotFound() {
  server.send(404, "text/plain", "QUOD GRATIS ASSERITUR GRATIS NEGATUR");
}

void sendOkReply() {
  server.send(200, "text/html", "OK");
}
