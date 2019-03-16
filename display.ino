void rollCurrentBufferDown(uint8_t *currentBuffer, uint8_t *nextBuffer, int8_t currentBufferRow) {
  for(int i = 0; i < COL_SIZE; i++) {
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
  uint8_t len;
  uint32_t now = millis();

  if(now > roll->referenceTime + roll->frameRate) {
    roll->referenceTime += roll->frameRate;
    
    if(roll->currentDigit != roll->nextDigit) {
      roll->currentBufferRow = 7;
      roll->currentDigit = roll->nextDigit;
      roll->len = mx.getChar(String(roll->nextDigit)[0], sizeof(roll->nextBuffer)/sizeof(roll->nextBuffer[0]), roll->nextBuffer);
    }

    if(roll->currentBufferRow > -1) {
      rollCurrentBufferDown(roll->currentBuffer, roll->nextBuffer, roll->currentBufferRow);

      mx.setBuffer(roll->col, roll->len, roll->currentBuffer);

      roll->currentBufferRow--;
    }
  }
}

int printTime() {
  uint8_t second = getSecond(), second1 = floor(second / 10), second2 = second % 10;

  if(rollSecond1.nextDigit != second1) {
    rollSecond1.nextDigit = second1;
  }

  if(rollSecond2.nextDigit != second2) {
    rollSecond2.nextDigit = second2;
  }

  rollDown(&rollSecond1);
  rollDown(&rollSecond2);
}

