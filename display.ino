void startMatrix() {
  mx.begin();
  mx.clear();
  mx.setFont(_font);  
  setBrightness(0);

  mx.setColumn(3, B00011000);
  mx.setColumn(4, B00011000);
  mx.setColumn(11, B00011000);
  mx.setColumn(12, B00011000);
  mx.setColumn(19, B00011000);
  mx.setColumn(20, B00011000);
  
  mx.update();

  roll1.col = 21;
  roll2.col = 13;
  roll3.col = 5;
}

// Used to display the initial animation while waiting for the clock to sync with NTP
void displayInit() {
  uint32_t now = millis();
  
  static uint16_t DISPLAY_TIME = 1500;
  static uint32_t referenceTime = now - DISPLAY_TIME;
  static IPAddress ip = WiFi.localIP();
  static int currentIpSegment = 0;
  
  uint8_t segment;
  char segmentChar[4];

  if(now < referenceTime + DISPLAY_TIME) {
    mx.update(MD_MAX72XX::OFF);
    
    rollDown(&roll1);
    rollDown(&roll2);
    rollDown(&roll3);
    
    mx.update(MD_MAX72XX::ON);

    return;
  }

  // Make sure that the full IP address is displayed at least once
  if(currentIpSegment > 3) {
    if(now > referenceTime + DISPLAY_TIME) {
      if (isTimeSet) {
        mx.clear();
        delay(500);
        
        roll1.col = 23;
        roll2.col = 18;
        roll3.col = 11;
        roll4.col = 6;

        roll1.currentDigit = 99;
        roll2.currentDigit = 99;
        roll3.currentDigit = 99;
        roll4.currentDigit = 99;

        now = millis();

        roll1.referenceTime = now;
        roll2.referenceTime = now;
        roll3.referenceTime = now;
        roll4.referenceTime = now;
        
        state = ST_CLOCK;

        return;
      } else {
        currentIpSegment = 0;
      }
    } else {
      return;
    }
  }

  segment = ip[currentIpSegment];
  referenceTime += DISPLAY_TIME;

  sprintf(segmentChar, "%03d", segment);

  roll1.nextDigit = int(segmentChar[0]) - 48;
  roll2.nextDigit = int(segmentChar[1]) - 48;
  roll3.nextDigit = int(segmentChar[2]) - 48;

  currentIpSegment++;
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
