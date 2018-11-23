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
  drawSprite(secondsBar, (NUM_DEVICES *  8) - 1, 1);

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
      drawSprite(font[c], cursor, 1);
      cursor += 2;
    } else {
      drawSprite(font[c], cursor, 4);
      cursor += 5;
    }
  }
}

void drawSprite(byte* sprite, int x, int width) {
  byte mask;
  
  for(int iy = 0; iy < ROWS; iy++) {
    mask = B1 << width - 1;
    
    for(int ix = 0; ix < width; ix++) {
      lmd.setPixel(x + ix, iy, (bool)(sprite[iy] & mask));
  
      mask = mask >> 1;
    }
  }
}

