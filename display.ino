void createSecondsBar(uint8_t second, uint8_t col) {
  uint8_t numBars, REFRESH_RATE = 100;
  static uint32_t referenceTime;
  uint32_t now = millis();

  if(now > referenceTime + REFRESH_RATE) {
    referenceTime += REFRESH_RATE;
    numBars = floor(second / (60.0 / 9.0)); // 9 states: 8 lights + no light
    mx.setColumn(col, 255 - (B11111111 >> numBars));
  }
}

void rollCurrentBufferDown(uint8_t *currentBuffer, uint8_t *nextBuffer, int8_t currentBufferRow) {
  for(uint8_t i = 0; i < COL_SIZE; i++) {
    // Scroll down
    currentBuffer[i] <<= 1;

    // For each column in nextBuffer, looking at the row related to currentBufferRow if there's a point.
    // If so, set a point at the top of the currentBuffer for the same column
    if(nextBuffer[i] & (1 << currentBufferRow)) {
      currentBuffer[i] |= 1;
    }
  }
}

void rollDown(ROLL *roll) {
  uint8_t REFRESH_RATE = 30;
  uint32_t now = millis();

  if(now > roll->referenceTime + REFRESH_RATE) {
    roll->referenceTime += REFRESH_RATE;
    
    if(roll->currentDigit != roll->nextDigit) {
      roll->currentBufferRow = 7;
      roll->currentDigit = roll->nextDigit;
      roll->width = mx.getChar(String(roll->nextDigit)[0], sizeof(roll->nextBuffer)/sizeof(roll->nextBuffer[0]), roll->nextBuffer);
    }

    if(roll->currentBufferRow > -1) {
      rollCurrentBufferDown(roll->currentBuffer, roll->nextBuffer, roll->currentBufferRow);

      mx.setBuffer(roll->col, roll->width, roll->currentBuffer);

      roll->currentBufferRow--;
    }
  }
}

void blinkColon(uint8_t col) {
  static bool isOn = false;
  static uint32_t referenceTime;
  uint32_t now = millis();

  if(now > referenceTime + 1000) {
    referenceTime += 1000;
    isOn = !isOn;

    if(isOn) {
      mx.setColumn(col, 0x24);
    } else {
      mx.setColumn(col, 0);
    }
  }
}

void displayClockShort() {
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
  blinkColon(17);
  rollDown(&rollMinute1);
  rollDown(&rollMinute2);
  createSecondsBar(second, 4);

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

void displayClock() {
  switch(setUp.clockFormat) {
    case CLOCK_FORMAT::SHORT:
      displayClockShort();
      break;
    case CLOCK_FORMAT::FULL:
      displayClockFull();
      break;
  }
}

