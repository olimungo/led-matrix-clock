void displayTimer() {
  switch(timer.mode) {
    case MD_OFF:
      updateDisplay();     
      break;
    case MD_RUN:
      runTimer();
      break;
  }
}

void timerStart() {
  state = ST_TIMER;
  chrono.mode= MD_OFF;
  
  if (timer.mode == MD_OFF) {
    uint32_t now = millis();
    
    state = ST_TIMER;
    timer.mode = MD_RUN;

    timer.targetTime = now +
      timer.minute1 * 10.0 * 60.0 * 1000.0 +
      timer.minute2 * 60.0 * 1000.0 +
      timer.second1 * 10.0 * 1000.0 +
      timer.second2 * 1000.0;
  }

  sendOkReply();
}

void timerPause() {
  state == ST_TIMER;
  chrono.mode= MD_OFF;
  
  if (timer.mode == MD_RUN) {
    timer.mode = MD_PAUSE;
    timer.referencePausedTime = millis();
  } else if ((timer.mode == MD_PAUSE)) {
    timer.mode = MD_RUN;
    timer.targetTime += millis() - timer.referencePausedTime;
  }

  setTimer();

  sendOkReply();
}

void timerReset() {
  uint32_t now = millis();
  
  state = ST_TIMER;
  chrono.mode= MD_OFF;
  timer.mode = MD_OFF;
  
  timer.minute1 = timer.minute2 = timer.second1 = timer.second2 = 0;
  
  roll1.nextDigit = timer.minute1;
  roll2.nextDigit = timer.minute2;
  roll3.nextDigit = timer.second1;
  roll4.nextDigit = timer.second2;

  roll1.referenceTime = now;
  roll2.referenceTime = now;
  roll3.referenceTime = now;
  roll4.referenceTime = now;

  sendTimerReply();
}

void timerMinute1OneMore() {
  if (timer.minute1 < 9) {
    timer.minute1 += 1;
  }

  setTimer();

  sendTimerReply();
}

void timerMinute1OneLess() {
  if (timer.minute1 > 0) {
    timer.minute1 -= 1;
  }

  setTimer();

  sendTimerReply();
}

void timerMinute2OneMore() {
  if (timer.minute2 < 9) {
    timer.minute2 += 1;
  }

  setTimer();

  sendTimerReply();
}

void timerMinute2OneLess() {
  if (timer.minute2 > 0) {
    timer.minute2 -= 1;
  }

  setTimer();

  sendTimerReply();
}

void timerSecond1OneMore() {
  if (timer.second1 < 5) {
    timer.second1 += 1;
  }

  setTimer();

  sendTimerReply();
}

void timerSecond1OneLess() {
  if (timer.second1 > 0) {
    timer.second1 -= 1;
  }

  setTimer();

  sendTimerReply();
}

void timerSecond2OneMore() {
  if (timer.second2 < 9) {
    timer.second2 += 1;
  }

  setTimer();

  sendTimerReply();
}

void timerSecond2OneLess() {
  if (timer.second2 > 0) {
    timer.second2 -= 1;
  }

  setTimer();

  sendTimerReply();
}

void timerMinutesFiveMore() {
  uint8_t minutes = timer.minute1 * 10 + timer.minute2 + 5;
  
  if (minutes < 100) {
    timer.minute1 = floor(minutes / 10L);
    timer.minute2 = minutes % 10;
  }

  setTimer();

  sendTimerReply();
}

void timerMinutesFiveLess() {
  int minutes = timer.minute1 * 10 + timer.minute2 - 5;
  
  if (minutes > 0) {
    timer.minute1 = floor(minutes / 10L);
    timer.minute2 = minutes % 10;
  }

  setTimer();

  sendTimerReply();
}

void runTimer() {
  int32_t remainingTime = timer.targetTime - millis();
  uint8_t minute, second;

  minute = remainingTime / 1000 / 60 % 60;
  second = remainingTime / 1000 % 60;

  roll1.nextDigit = floor(minute / 10);
  roll2.nextDigit = minute % 10;
  roll3.nextDigit = floor(second / 10);
  roll4.nextDigit = second % 10;

  updateDisplay();
}

void sendTimerReply() {
  char result[200];

  sprintf(result, "{ \"minute1\": \"%d\", \"minute2\": \"%d\", \"second1\": \"%d\", \"second2\": \"%d\" }",
    timer.minute1, timer.minute2, timer.second1, timer.second2);
    
  server.send(200, "application/json", result);
}

void setTimer() { 
  state = ST_TIMER;
  chrono.mode = MD_OFF;

  roll1.nextDigit = timer.minute1;
  roll2.nextDigit = timer.minute2;
  roll3.nextDigit = timer.second1;
  roll4.nextDigit = timer.second2;

  updateRollReferenceTime(millis());

  updateDisplay();
}
