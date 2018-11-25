void displayTimeShort() {
  bool evenSecond = getSecond() % 2 == 0;
  char timeFormatted[15], format[15];

  if (evenSecond) {
    strcpy(format, "%02d:%02d");
  } else {
    strcpy(format, "%02d o%02d");
  }
  
  snprintf(timeFormatted, sizeof(timeFormatted), format, getHour(), getMinute());

  lmd.clear();
  
  displayTimeString(timeFormatted, 4);

  byte secondsBar[ROWS];
  createSecondsBar(secondsBar);
  drawClockChar(secondsBar, (NUM_DEVICES *  8) - 1, 1);

  lmd.display();
}

void displayTimeFull() {
  char timeFormatted[15], format[15];

  strcpy(format, "%02d:%02do%02d");
  
  snprintf(timeFormatted, sizeof(timeFormatted), format, getHour(), getMinute(), getSecond());

  lmd.clear();
  
  displayTimeString(timeFormatted, 0);

  lmd.display();
}

void displayTimeString(char* text, int cursor) {
  int len = strlen(text);

  for(int idx = 0; idx < len; idx ++) {
    int c = text[idx] - 48;

    if (c == -16) { // SPACE
      cursor += 1;
    } else if (c == 72) { // x
      cursor += 5;
    } else if (c == 63) { // o
      cursor += 1;
    } else if (c == 10) { // :
      drawClockChar(clockFonts[c], cursor, 1);
      cursor += 2;
    } else {
      drawClockChar(clockFonts[c], cursor, 4);
      cursor += 5;
    }
  }
}

void drawClockChar(byte* sprite, int x, int width) {
  byte mask;
  
  for(int iy = 0; iy < ROWS; iy++) {
    mask = B1 << width - 1;
    
    for(int ix = 0; ix < width; ix++) {
      lmd.setPixel(x + ix, iy, (bool)(sprite[iy] & mask));
  
      mask = mask >> 1;
    }
  }
}

void displaySetClock() {
  static bool isNumberHidden = false;
  static unsigned long previousMillis = 0;
  
  unsigned long currentMillis = millis();
  char timeFormatted[10];

  if (currentMillis - previousMillis > 500) {
    previousMillis = currentMillis;
    isNumberHidden = !isNumberHidden;
  }
  
  if (isNumberHidden) {
    switch(modeSetClock) {
      case SETCLOCK_HOUR1:
        snprintf(timeFormatted, sizeof(timeFormatted), "x%d %d%d", savedHour2, savedMinute1, savedMinute2);
        break;
      case SETCLOCK_HOUR2:
        snprintf(timeFormatted, sizeof(timeFormatted), "%dx %d%d", savedHour1, savedMinute1, savedMinute2);
        break;
      case SETCLOCK_MINUTE1:
        snprintf(timeFormatted, sizeof(timeFormatted), "%d%d x%d", savedHour1, savedHour2, savedMinute2);
        break;
      case SETCLOCK_MINUTE2:
        snprintf(timeFormatted, sizeof(timeFormatted), "%d%d %dx", savedHour1, savedHour2, savedMinute1);
        break;
    }
  } else {
    snprintf(timeFormatted, sizeof(timeFormatted), "%d%d %d%d", savedHour1, savedHour2, savedMinute1, savedMinute2);
  }

  lmd.clear();
  
  displayTimeString(timeFormatted, 4);

  lmd.display();
}

void displayChrono() {
  bool evenSecond = getSecond() % 2 == 0;
  char timeFormatted[15], format[15];
  int hour = 0, minute = 0, second = 0;
  unsigned long delta = millis() - chronoMillis;

  if (chronoMillis > 0) {
    if (chronoPause > 0) {
      delta -= millis() - chronoPause;
    }

    hour = delta / 1000 / 60 / 60;
    minute = delta / 1000 / 60 % 60;
    second = delta / 1000 % 60;
    
    evenSecond = false;
  }

  if (evenSecond) {
    strcpy(format, "xx:xxoxx");
    snprintf(timeFormatted, sizeof(timeFormatted), format);
  } else {
    strcpy(format, "%02d:%02do%02d");
    snprintf(timeFormatted, sizeof(timeFormatted), format, hour, minute, second);
  }

  lmd.clear();
  
  displayTimeString(timeFormatted, 0);

  lmd.display();
}

