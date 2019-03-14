int  printChar(char character, int col) {
  uint8_t buffer[8], len, i = 0;

  len = mx.getChar(character, sizeof(buffer)/sizeof(buffer[0]), buffer);

  mx.setChar(col, character);

  return len;
}

void printTime() {
  char text[15];
  uint8_t len, i = 0, width, col = COL_SIZE * NUM_DEVICES - 1;
  
  sprintf(text, "%02d:%02d %02d", getHour(), getMinute(), getSecond());
  len = strlen(text);

  while(i < len) {
    width = printChar(text[i], col);
    col -= width;
    i++;
  }
}

