void displayChrono() { 
  switch(chrono.state) {
    case STATE_CHRONO::SC_INTRO:
      rollDownChrono();
      chrono.state++;
      
      break;
    case STATE_CHRONO::SC_PAUSE:
      break;
    case STATE_CHRONO::SC_RUN:
      runChrono();
      
      break;
  }
}

void rollDownChrono() {
  uint8_t nextBuffer[MAX_COLS];
  char clock[20] = "00:00 00";

  rollHour1.currentDigit = rollHour1.nextDigit = 0;
  rollHour2.currentDigit = rollHour2.nextDigit = 0;
  rollMinute1.currentDigit = rollMinute1.nextDigit = 0;
  rollMinute2.currentDigit = rollMinute2.nextDigit = 0;
  rollSecond1.currentDigit = rollSecond1.nextDigit = 0;
  rollSecond2.currentDigit = rollSecond2.nextDigit = 0;

  createBuffer(clock, nextBuffer);
  switchBuffers(nextBuffer);
}

void runChrono() {
  uint32_t ellapsedTime = millis() - chrono.referenceTime;
  uint8_t hour, minute, second;

  hour = ellapsedTime / 1000 / 60 / 60;
  minute = ellapsedTime / 1000 / 60 % 60;
  second = ellapsedTime / 1000 % 60;

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

