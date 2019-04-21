void clockDisplay() {
  state = ST_CLOCK;
  chrono.mode = MD_OFF;
  timer.mode = MD_OFF;

  sendOkReply();
}

void displayClock() {
  static uint32_t referenceTime;
  uint32_t REFRESH_RATE = 1000;
  uint32_t now = millis();
  uint8_t hour = getHour(), hour1 = floor(hour / 10), hour2 = hour % 10;
  uint8_t minute = getMinute(), minute1 = floor(minute / 10), minute2 = minute % 10;
  uint8_t second = getSecond(), second1 = floor(second / 10), second2 = second % 10;
  uint8_t secondsBar;
  
  roll1.nextDigit = hour1;
  roll2.nextDigit = hour2;
  roll3.nextDigit = minute1;
  roll4.nextDigit = minute2;

  mx.update(MD_MAX72XX::OFF);

  rollDown(&roll1);
  rollDown(&roll2);
  blinkColon(13);
  rollDown(&roll3);
  rollDown(&roll4);

  if(now > referenceTime + REFRESH_RATE) {
    referenceTime += REFRESH_RATE;
    secondsBar = createSecondsBar(second);
    mx.setColumn(0, secondsBar);
  }

  mx.update(MD_MAX72XX::ON);
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
