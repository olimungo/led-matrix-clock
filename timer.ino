void displayTimer() { 
  switch(timer.state) {
    case STATE_TIMER::ST_INTRO:
      rollDownTimer();
      timer.state++;
      
      break;
    case STATE_TIMER::ST_SET:
      setTimer();
      
      break;
    case STATE_TIMER::ST_RUN:
      runTimer();
      
      break;
    case STATE_TIMER::ST_PAUSE:
      break;
  }
}

void rollDownTimer() {
  uint8_t nextBuffer[MAX_COLS];
  char clock[20];

  if(setUp.state == STATE::TIMER_1) {
    sprintf(clock, "00:%02d 00", timer.fiveMinuteCount);
    
    rollHour1.currentDigit = rollHour1.nextDigit = 0;
    rollHour2.currentDigit = rollHour2.nextDigit = 0;
    rollMinute1.currentDigit = rollMinute1.nextDigit = floor(timer.fiveMinuteCount / 10);
    rollMinute2.currentDigit = rollMinute2.nextDigit = timer.fiveMinuteCount % 10;
    rollSecond1.currentDigit = rollSecond1.nextDigit = 0;
    rollSecond2.currentDigit = rollSecond2.nextDigit = 0;
  } else {
    sprintf(clock, "%d%d:%d%d %d%d", timer.hour1, timer.hour2, timer.minute1, timer.minute2,
      timer.second1, timer.second2);
  
    rollHour1.currentDigit = rollHour1.nextDigit = timer.hour1;
    rollHour2.currentDigit = rollHour2.nextDigit = timer.hour2;
    rollMinute1.currentDigit = rollMinute1.nextDigit = timer.minute1;
    rollMinute2.currentDigit = rollMinute2.nextDigit = timer.minute2;
    rollSecond1.currentDigit = rollSecond1.nextDigit = timer.second1;
    rollSecond2.currentDigit = rollSecond2.nextDigit = timer.second2;
  }

  createBuffer(clock, nextBuffer);
  switchBuffers(nextBuffer);
}

void setTimer() {
  if(setUp.state == STATE::TIMER_1) {
    setTimer1();
  } else {
    setTimer2();
  }
}

void setTimer1() {
  uint8_t nextBuffer[MAX_COLS];
  char clock[50];
  bool evenSecond = (millis() / 500) % 2 == 0;

  if (evenSecond) {
    sprintf(clock, "00:%02d 00", timer.fiveMinuteCount);
  } else {
    sprintf(clock, "00:           00");
  }

  createBuffer(clock, nextBuffer);
  mx.setBuffer(MAX_COLS - 1, MAX_COLS, nextBuffer);
}

void setTimer2() {
  uint8_t nextBuffer[MAX_COLS];
  char clock[50];
  bool evenSecond = (millis() / 500) % 2 == 0;

  if (evenSecond) {
    switch(timer.stateSelect) {
      case STATE_TIMER_SELECT::HOUR1:
        sprintf(clock, "     %d:%d%d %d%d", timer.hour2, timer.minute1, timer.minute2,
          timer.second1, timer.second2);
        break;
      case STATE_TIMER_SELECT::HOUR2:
        sprintf(clock, "%d     :%d%d %d%d", timer.hour1, timer.minute1, timer.minute2,
          timer.second1, timer.second2);
        break;
      case STATE_TIMER_SELECT::MINUTE1:
        sprintf(clock, "%d%d:     %d %d%d", timer.hour1, timer.hour2, timer.minute2,
          timer.second1, timer.second2);
        break;
      case STATE_TIMER_SELECT::MINUTE2:
        sprintf(clock, "%d%d:%d      %d%d", timer.hour1, timer.hour2, timer.minute1,
          timer.second1, timer.second2);
        break;
      case STATE_TIMER_SELECT::SECOND1:
        sprintf(clock, "%d%d:%d%d      %d", timer.hour1, timer.hour2, timer.minute1, timer.minute2,
          timer.second2);
        break;
      case STATE_TIMER_SELECT::SECOND2:
        sprintf(clock, "%d%d:%d%d %d    ", timer.hour1, timer.hour2, timer.minute1, timer.minute2,
          timer.second1);
        break;
    }
  } else {
    sprintf(clock, "%d%d:%d%d %d%d", timer.hour1, timer.hour2, timer.minute1, timer.minute2,
      timer.second1, timer.second2);
  }

  createBuffer(clock, nextBuffer);
  mx.setBuffer(MAX_COLS - 1, MAX_COLS, nextBuffer);
}

void runTimer() {
  uint32_t remainingTime = timer.targetTime - millis();
  uint8_t hour, minute, second;

  if(remainingTime <= 0) {
    timer.targetTime = 0;
    timer.state++;
    
    return;
  }

  hour = remainingTime / 1000 / 60 / 60;
  minute = remainingTime / 1000 / 60 % 60;
  second = remainingTime / 1000 % 60;

  rollHour1.nextDigit = floor(hour / 10);
  rollHour2.nextDigit = hour % 10;
  rollMinute1.nextDigit = floor(minute / 10);
  rollMinute2.nextDigit = minute % 10;
  rollSecond1.nextDigit = floor(second / 10);
  rollSecond2.nextDigit = second % 10;

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


