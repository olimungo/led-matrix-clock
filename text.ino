void displayEasterEgg() {  
  char item[MAX_CHAR_PER_ITEM];
  int itemNumber;
    
  itemNumber = random(sizeof(easterEggTexts) / MAX_CHAR_PER_ITEM);

  memcpy_P(&item, &easterEggTexts[itemNumber], MAX_CHAR_PER_ITEM);
  
  scrollText(item);
}

void displayMidnightMode() {
  static unsigned int itemNumber = 0;
  char item[MAX_CHAR_PER_ITEM];

  memcpy_P(&item, &midnightModeTexts[itemNumber], MAX_CHAR_PER_ITEM);
  
  scrollText(item);

  if (++itemNumber >= sizeof(midnightModeTexts) / MAX_CHAR_PER_ITEM) {
    itemNumber = 0;
  }
}

void displaySetup() {
  if (modeSetup == SETUP_TIME_SHORT) {
    scrollText(TXT_SETUP_TIME_SHORT);
  } else {
    scrollText(TXT_SETUP_TIME_FULL);
  }
}

void scrollText(char* text) {
  int x = (NUM_DEVICES * 8) - 1;
  int len = strlen(text);

  lmd.clear();

  while (x >= len * -8) {
    drawString(text, len, x);
    lmd.display();
    delay(ANIM_DELAY);

    if (checkButton()) {
      break;
    }

    x--;
  }
}

void drawString(char* text, int len, int x) {
  byte letter[8];
  
  for (int idx = 0; idx < len; idx ++) {
    int c = text[idx] - 32;

    // Stop if char is outside visible area
    if (x + idx * 7  > (NUM_DEVICES * 8) - 1) {
      return;
    }

    // Only draw if char is visible
    if (7 + x + idx * 7 > 0) {
      memcpy_P(&letter, &alphabetInProgmem[c], ROWS);
      drawSprite(letter, x + idx * 7, 0, 7, 8);
    }
  }
}

void drawSprite( byte* sprite, int x, int y, int width, int height ) {
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;
  
  for (int iy = 0; iy < height; iy++) {
    for (int ix = 0; ix < width; ix++) {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));

      // Shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // Reset column mask
    mask = B10000000;
  }
}
