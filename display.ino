void displayTitle(char *title) {
  uint8_t nextBuffer[MAX_COLS], width;

  width = createTitle(title, nextBuffer);
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
  uint8_t currentBuffer[MAX_COLS], REFRESH_RATE = 30;

  mx.getBuffer(31, MAX_COLS, currentBuffer);

  for(int row = 7; row >= 0; row--) {
    mx.update(MD_MAX72XX::OFF);
    
    rollBufferDown(currentBuffer, nextBuffer, row, MAX_COLS);
    mx.setBuffer(31, MAX_COLS, currentBuffer);

    mx.update(MD_MAX72XX::ON);

    delay(REFRESH_RATE);
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
  uint32_t now = millis(), REFRESH_RATE = 35;

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

void setRollsReferenceTime(uint32_t time) {
  rollHour1.referenceTime = rollHour2.referenceTime = rollMinute1.referenceTime =
    rollMinute2.referenceTime = rollSecond1.referenceTime = rollSecond2.referenceTime = time;
}

