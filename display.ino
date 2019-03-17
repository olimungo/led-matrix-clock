void addSecondsBar(uint8_t second, uint8_t col) {
  static uint32_t referenceTime;
  uint8_t numBars;
  uint32_t now = millis(), REFRESH_RATE = 100;

  if(now < referenceTime + REFRESH_RATE) {
    return;
  }
  
  referenceTime += REFRESH_RATE;
  numBars = floor(second / (60.0 / 9.0)); // 9 states: 8 lights + no light
  mx.setColumn(col, 255 - (B11111111 >> numBars));
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
  addSecondsBar(second, 4);

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

void setClockShort() {
  rollHour1.col = 27;
  rollHour2.col = 22;
  rollMinute1.col = 15;
  rollMinute2.col = 10;
}

void setClockFull() {
  rollHour1.col = 31;
  rollHour2.col = 26;
  rollMinute1.col = 19;
  rollMinute2.col = 14;
  rollSecond1.col = 8;
  rollSecond2.col = 3;
}

void displayTitle(char *title) {
  uint8_t nextBuffer[MAX_COLS], width;

  width = createTitle(title, nextBuffer);
  centerBuffer(nextBuffer, width);
  switchBuffers(nextBuffer);
}

void displayTitleClock() {
  uint8_t hour = getHour(), hour1 = floor(hour / 10), hour2 = hour % 10;
  uint8_t minute = getMinute(), minute1 = floor(minute / 10), minute2 = minute % 10;
  uint8_t second = getSecond(), second1 = floor(second / 10), second2 = second % 10;
  uint8_t nextBuffer[MAX_COLS];
  uint8_t width;
  char clock[20];

  if(setUp.clockFormat == CLOCK_FORMAT::SHORT) {
    sprintf(clock, "%d%d:%d%d  ", hour1, hour2, minute1, minute2);
  } else {
    sprintf(clock, "%d%d:%d%d %d    ", hour1, hour2, minute1, minute2, second1);
  }

  // Update the rolls so there's no graphic glitch
  rollHour1.currentDigit = hour1;
  rollHour2.currentDigit = hour2;
  rollMinute1.currentDigit = minute1;
  rollMinute2.currentDigit = minute2;
  rollSecond1.currentDigit = second1;
  rollSecond2.currentDigit = second2;
  rollHour1.nextDigit = hour1;
  rollHour2.nextDigit = hour2;
  rollMinute1.nextDigit = minute1;
  rollMinute2.nextDigit = minute2;
  rollSecond1.nextDigit = second1;
  rollSecond2.nextDigit = 99;

  for(uint8_t i = 0; i < 4; i++) {
    rollSecond2.currentBuffer[i] = 0;
  }

  width = createTitle(clock, nextBuffer);
  centerBuffer(nextBuffer, width);
  switchBuffers(nextBuffer);
}

uint8_t createTitle(char *title, char *buffer) {
  uint8_t titleLength = strlen(title), charWidth, width = 0;
  uint8_t character[COL_SIZE];

  // Add all the characters to a buffer
  for(uint8_t i = 0; i < titleLength; i++) { 
    // Getting back one character in the font library
    charWidth = mx.getChar(title[i], sizeof(character)/sizeof(character[0]), character);

    // Add the character to the buffer
    for(uint8_t j = 0; j < charWidth; j++) {
      buffer[width++] = character[j];

      // Stop if no more space in the buffer
      if(width > MAX_COLS - 1) {
        break;
      }
    }

    // Stop if no more space in the buffer
    if(width > MAX_COLS - 1) {
      break;
    } else {
      // Add a blank column as character separator... if the current char is not a space character
      if(title[i] != 32) {
        buffer[width++] = 0;
      }
    }
  }
  
  return width;
}

void centerBuffer(char *buffer, uint8_t width) {
  uint8_t leftCols = ceil((MAX_COLS - width) / 2.0);
  uint8_t rightCols = floor((MAX_COLS - width) / 2.0);
  uint8_t nextBuffer[MAX_COLS], pos = 0;

  while(leftCols-- > 0) {
    nextBuffer[pos++] = 0;
  }

  for(uint8_t i = 0; i < width; i++) {
    nextBuffer[pos++] = buffer[i];
  }

  while(rightCols-- > 0) {
    nextBuffer[pos++] = 0;
  }

  for(uint8_t j = 0; j < MAX_COLS; j++) {
    buffer[j] = nextBuffer[j];
  }
}

void switchBuffers(char *nextBuffer) {
  uint8_t currentBuffer[MAX_COLS];

  mx.getBuffer(31, MAX_COLS, currentBuffer);

  for(int row = 7; row >= 0; row--) {
    mx.update(MD_MAX72XX::OFF);
    
    rollBufferDown(currentBuffer, nextBuffer, row, MAX_COLS);
    mx.setBuffer(31, MAX_COLS, currentBuffer);

    mx.update(MD_MAX72XX::ON);

    delay(30);
  }
}

void rollBufferDown(uint8_t *currentBuffer, uint8_t *nextBuffer, int8_t currentBufferRow, uint8_t width) {
  for(uint8_t i = 0; i < width; i++) {
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
  uint32_t now = millis(), REFRESH_RATE = 30;

  if(now < roll->referenceTime + REFRESH_RATE) {
    return;
  }
  
  roll->referenceTime += REFRESH_RATE;
  
  if(roll->currentDigit != roll->nextDigit) {
    roll->currentBufferRow = 7;
    roll->currentDigit = roll->nextDigit;
    roll->width = mx.getChar(String(roll->nextDigit)[0], sizeof(roll->nextBuffer)/sizeof(roll->nextBuffer[0]), roll->nextBuffer);
  }

  if(roll->currentBufferRow > -1) {
    rollBufferDown(roll->currentBuffer, roll->nextBuffer, roll->currentBufferRow, COL_SIZE);

    mx.setBuffer(roll->col, roll->width, roll->currentBuffer);

    roll->currentBufferRow--;
  }
}

