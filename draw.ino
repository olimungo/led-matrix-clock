void drawTime() {
  bool evenSecond = getSecond() % 2 == 0;
  char timeFormatted[10], format[10];

  if (evenSecond) {
    strcpy(format, "%02d:%02d");
  } else {
    strcpy(format, "%02d %02d");
  }
  
  snprintf(timeFormatted, sizeof(timeFormatted), format, getHour(), getMinute());

  // Serial.println(timeFormatted);

  lmd.clear();
  
  drawString(timeFormatted);

  byte secondsBar[ROWS];
  createSecondsBar(secondsBar);
  drawSprite(secondsBar, 23, 1);

  lmd.display();
}

void drawString(char* text) {
  int cursor = 0;
  int len = strlen(text);

  for(int idx = 0; idx < len; idx ++) {
    int c = text[idx] - 48;

    if (c == -16) { // SPACE
      cursor += 2;
    } else if (c == 72) { // x
      cursor += 5;
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

void createSecondsBar(byte* secondsBar) {
  int numBars = floor(getSecond() / (60.0 / 9.0)); // 9 states: 8 lights + no light
 
  for (int row = ROWS - 1; row >= 0; row--) {
    if (numBars > 0) {
      secondsBar[row] = 0x1;
      numBars--;
    } else {
      secondsBar[row] = 0x0;
    }
  }
}
