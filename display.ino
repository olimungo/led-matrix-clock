void startMax7219Driver() {
  mx.begin();
  mx.clear();
  mx.setFont(_font);  
  setBrightness(0);
  mx.update();

  roll1.col = 23;
  roll2.col = 18;
  roll3.col = 11;
  roll4.col = 6;
}

void setBrightness(uint8_t value) {
  mx.control(MD_MAX72XX::INTENSITY, value);
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
  static uint8_t REFRESH_RATE = 35;
  uint32_t now = millis();

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

void updateDisplay() {
  mx.update(MD_MAX72XX::OFF);
  
  rollDown(&roll1);
  rollDown(&roll2);
  blinkColon(13);
  rollDown(&roll3);
  rollDown(&roll4);

  mx.setColumn(0, 0);

  mx.update(MD_MAX72XX::ON);
}
