void displayClock() {
  switch(setUp.clockFormat) {
    case CLOCK_FORMAT::SHORT:
      displayClockShort();
      break;
    case CLOCK_FORMAT::LARGE:
      displayClockFull();
      break;
  }
}

uint8_t createSecondsBar(uint8_t second) {  
  uint8_t numBars;

  numBars = floor(second / (60.0 / 9.0)); // 9 states: 8 lights + no light
  return 255 - (B11111111 >> numBars);
}

void blinkColon(uint8_t col) {
  static bool isOn = false;
  static uint32_t referenceTime;
  uint32_t REFRESH_RATE = 1000;
  uint32_t now = millis();

  if(now < referenceTime + REFRESH_RATE) {
    return;
  }
  
  referenceTime += REFRESH_RATE;
  isOn = !isOn;

  if(isOn) {
    mx.setColumn(col, 0x24);
  } else {
    mx.setColumn(col, 0);
  }
}

void displayClockShort() {
  static uint32_t referenceTime;
  uint32_t REFRESH_RATE = 1000;
  uint32_t now = millis();
  uint8_t hour = getHour(), hour1 = floor(hour / 10), hour2 = hour % 10;
  uint8_t minute = getMinute(), minute1 = floor(minute / 10), minute2 = minute % 10;
  uint8_t second = getSecond(), second1 = floor(second / 10), second2 = second % 10;
  uint8_t secondsBar;
  
  rollHour1.nextDigit = hour1;
  rollHour2.nextDigit = hour2;
  rollMinute1.nextDigit = minute1;
  rollMinute2.nextDigit = minute2;
  rollSecond1.nextDigit = second1;
  rollSecond2.nextDigit = second2;

  mx.update(MD_MAX72XX::OFF);

  rollDown(&rollHour1);
  rollDown(&rollHour2);
  blinkColon(17);
  rollDown(&rollMinute1);
  rollDown(&rollMinute2);

  if(now > referenceTime + REFRESH_RATE) {
    referenceTime += REFRESH_RATE;
    secondsBar = createSecondsBar(second);
    mx.setColumn(4, secondsBar);
  }

  mx.update(MD_MAX72XX::ON);
}

void displayClockFull() {
  uint8_t hour = getHour(), hour1 = floor(hour / 10), hour2 = hour % 10;
  uint8_t minute = getMinute(), minute1 = floor(minute / 10), minute2 = minute % 10;
  uint8_t second = getSecond(), second1 = floor(second / 10), second2 = second % 10;
  
  rollHour1.nextDigit = hour1;
  rollHour2.nextDigit = hour2;
  rollMinute1.nextDigit = minute1;
  rollMinute2.nextDigit = minute2;
  rollSecond1.nextDigit = second1;
  rollSecond2.nextDigit = second2;

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

void setClockShort() {
  setUp.clockFormat = CLOCK_FORMAT::SHORT;
  
  rollHour1.col = 27;
  rollHour2.col = 22;
  rollMinute1.col = 15;
  rollMinute2.col = 10;
}

void setClockFull() {
  setUp.clockFormat = CLOCK_FORMAT::LARGE;
  
  rollHour1.col = 31;
  rollHour2.col = 26;
  rollMinute1.col = 19;
  rollMinute2.col = 14;
  rollSecond1.col = 8;
  rollSecond2.col = 3;
}

void displayTitleClock() {
  RTC_TIME rtcTime;
  
  getRtcTime(&rtcTime);

  // Update the rolls so there's no graphic glitch
  rollHour1.currentDigit = rtcTime.hour1;
  rollHour2.currentDigit = rtcTime.hour2;
  rollMinute1.currentDigit = rtcTime.minute1;
  rollMinute2.currentDigit = rtcTime.minute2;
  rollSecond1.currentDigit = rtcTime.second1;
  rollSecond2.currentDigit = rtcTime.second2;
  rollHour1.nextDigit = rtcTime.hour1;
  rollHour2.nextDigit = rtcTime.hour2;
  rollMinute1.nextDigit = rtcTime.minute1;
  rollMinute2.nextDigit = rtcTime.minute2;
  rollSecond1.nextDigit = rtcTime.second1;
  rollSecond2.nextDigit = 99;

  if(setUp.clockFormat == CLOCK_FORMAT::SHORT) {
    displayTitleClock("%d%d:%d%d", &rtcTime, ADD_SECONDS_BAR);
  } else {
    displayTitleClock("%d%d:%d%d %d    ", &rtcTime, NO_SECONDS_BAR);
  }
}

void displayTitleClock(char mask[50], RTC_TIME *rtcTime, bool addSecondsBar) {
  uint8_t nextBuffer[MAX_COLS], width, secondsBar;
  char clock[20];

  sprintf(clock, mask, rtcTime->hour1, rtcTime->hour2, rtcTime->minute1, rtcTime->minute2, rtcTime->second1, rtcTime->second2);

  for(uint8_t i = 0; i < 4; i++) {
    rollSecond2.currentBuffer[i] = 0;
  }

  width = createBuffer(clock, nextBuffer);

  if(addSecondsBar) {
    secondsBar = createSecondsBar(rtcTime->second);
    nextBuffer[width++] = 0;
    nextBuffer[width++] = secondsBar;
  }
  
  centerBuffer(nextBuffer, width);
  switchBuffers(nextBuffer);
}
