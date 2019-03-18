void displayTimer() {
  switch(state_timer) {
    case STATE_TIMER::INTRO:
      rollDownTimer();
      state_timer++;
    case STATE_TIMER::SET:
      setTimer();
      break;
    case STATE_TIMER::RUN:
      break;
    case STATE_TIMER::PAUSE:
      break;
  }
}

void rollDownTimer() {
  uint8_t nextBuffer[MAX_COLS];
  char clock[20];

  sprintf(clock, "00:%02d 00", fiveMinuteCount);

  createTitle(clock, nextBuffer);
  switchBuffers(nextBuffer);
}

void setTimer() {
  uint8_t nextBuffer[MAX_COLS];
  char clock[20];
  bool evenSecond = (millis() / 500) % 2 == 0;

  if (evenSecond) {
    sprintf(clock, "00:%02d 00", fiveMinuteCount);
  } else {
    sprintf(clock, "            %02d          ", fiveMinuteCount);
  }

  createTitle(clock, nextBuffer);
  mx.setBuffer(MAX_COLS - 1, MAX_COLS, nextBuffer);
}

void displayTimer2() {
  uint8_t hour = getHour(), hour1 = floor(hour / 10), hour2 = hour % 10;
  uint8_t minute = getMinute(), minute1 = floor(minute / 10), minute2 = minute % 10;
  uint8_t second = getSecond(), second1 = floor(second / 10), second2 = second % 10;
  
  rollHour1.nextDigit = 0;
  rollHour2.nextDigit = 0;
  rollMinute1.nextDigit = 0;
  rollMinute2.nextDigit = 5;
  rollSecond1.nextDigit = 0;
  rollSecond2.nextDigit = 0;

  mx.update(MD_MAX72XX::OFF);
  
  rollDown(&rollHour1);
  rollDown(&rollHour2);
  blinkColon(21);
  rollDown(&rollMinute1);
  rollDown(&rollMinute2);
  rollDown(&rollSecond1);
  rollDown(&rollSecond2);

  mx.update(MD_MAX72XX::ON);
}


