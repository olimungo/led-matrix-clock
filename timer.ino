void timerStart() {
  timer.mode = MODE::RUN;

  sendOkReply();
}

void timerPause() {
  timer.mode = MODE::PAUSE;

  sendOkReply();
}

void timerReset() {
  timer.mode = MODE::OFF;
  timer.minute1 = timer.minute2 = timer.second1 = timer.second2 = 0;

  sendTimerReply();
}

void timerMinute1OneMore() {
  if (timer.minute1 < 9) {
    timer.minute1 += 1;
  }

  sendTimerReply();
}

void timerMinute1OneLess() {
  if (timer.minute1 > 0) {
    timer.minute1 -= 1;
  }

  sendTimerReply();
}

void timerMinute2OneMore() {
  if (timer.minute2 < 9) {
    timer.minute2 += 1;
  }

  sendTimerReply();
}

void timerMinute2OneLess() {
  if (timer.minute2 > 0) {
    timer.minute2 -= 1;
  }

  sendTimerReply();
}

void timerSecond1OneMore() {
  if (timer.second1 < 5) {
    timer.second1 += 1;
  }

  sendTimerReply();
}

void timerSecond1OneLess() {
  if (timer.second1 > 0) {
    timer.second1 -= 1;
  }

  sendTimerReply();
}

void timerSecond2OneMore() {
  if (timer.second2 < 9) {
    timer.second2 += 1;
  }

  sendTimerReply();
}

void timerSecond2OneLess() {
  if (timer.second2 > 0) {
    timer.second2 -= 1;
  }

  sendTimerReply();
}

void timerMinutesFiveMore() {
  uint8_t minutes = timer.minute1 * 10 + timer.minute2 + 5;
  
  if (minutes < 100) {
    timer.minute1 = floor(minutes / 10L);
    timer.minute2 = minutes % 10;
  }

  sendTimerReply();
}

void timerMinutesFiveLess() {
  int minutes = timer.minute1 * 10 + timer.minute2 - 5;
  
  if (minutes > 0) {
    timer.minute1 = floor(minutes / 10L);
    timer.minute2 = minutes % 10;
  }

  sendTimerReply();
}

void sendTimerReply() {
  char result[200];

  sprintf(result, "{ \"minute1\": \"%d\", \"minute2\": \"%d\", \"second1\": \"%d\", \"second2\": \"%d\" }",
    timer.minute1, timer.minute2, timer.second1, timer.second2);
    
  server.send(200, "application/json", result);
}
