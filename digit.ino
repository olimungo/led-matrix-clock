void displayTime() {
  if (timeFormat == SETUP_TIME_SHORT || NUM_DEVICES < 4) {
    displayTimeShort();
  } else {
    displayTimeFull();
  }
}

void displayTimeShort() {
  bool evenSecond = getSecond() % 2 == 0;
  char timeFormatted[15], format[15];

  if (evenSecond) {
    strcpy(format, "%02d:%02d");
  } else {
    strcpy(format, "%02do%02d");
  }
  
  sprintf(timeFormatted, format, getHour(), getMinute());

  lmd.clear();

  if (NUM_DEVICES < 4) {
    displayTimeString(timeFormatted, 0);
  } else {
    displayTimeString(timeFormatted, 4);
  }

  byte secondsBar[ROWS];
  createSecondsBar(secondsBar);
  drawSprite(secondsBar, (NUM_DEVICES *  8) - 1, 1);

  lmd.display();
}

void displayTimeFull() {
  char timeFormatted[15];
  
  sprintf(timeFormatted, "%02d:%02d %02d", getHour(), getMinute(), getSecond());

  lmd.clear();
  
  displayTimeString(timeFormatted, 0);

  lmd.display();
}

void displaySetClock() {
  static bool blink = false;
  static unsigned long previousMillis = 0;
  
  unsigned long currentMillis = millis();
  char timeFormatted[10];

  if (currentMillis - previousMillis > 500) {
    previousMillis = currentMillis;
    blink = !blink;
  }
  
  if (blink) {
    switch(modeSetClock) {
      case SETCLOCK_HOUR1:
        sprintf(timeFormatted, "x%d:%d%d", savedHour2, savedMinute1, savedMinute2);
        break;
      case SETCLOCK_HOUR2:
        sprintf(timeFormatted, "%dx:%d%d", savedHour1, savedMinute1, savedMinute2);
        break;
      case SETCLOCK_MINUTE1:
        sprintf(timeFormatted, "%d%d:x%d", savedHour1, savedHour2, savedMinute2);
        break;
      case SETCLOCK_MINUTE2:
        sprintf(timeFormatted, "%d%d:%dx", savedHour1, savedHour2, savedMinute1);
        break;
    }
  } else {
    sprintf(timeFormatted, "%d%d:%d%d", savedHour1, savedHour2, savedMinute1, savedMinute2);
  }

  lmd.clear();

  if (NUM_DEVICES < 4) {
    displayTimeString(timeFormatted, 0);
  } else {
    displayTimeString(timeFormatted, 5);
  }

  lmd.display();
}

void displayChrono() {
  static bool blink = false;
  static unsigned long previousMillis = 0;
  
  unsigned long currentMillis = millis();
  unsigned long delta = currentMillis - chronoMillis;
  
  char timeFormatted[15], format[15];
  unsigned int hours = 0, hour1 = 0, hour2 = 0, minutes = 0,
    minute1 = 0, minute2 = 0, seconds = 0, second1 = 0, second2 = 0;

  if (currentMillis - previousMillis > 500) {
    previousMillis = currentMillis;
    blink = !blink;
  }

  if (chronoMillis > 0) {
    if (chronoPause > 0) {
      delta -= millis() - chronoPause;
    }

    hours = delta / 1000 / 60 / 60;
    minutes = delta / 1000 / 60 % 60;
    seconds = delta / 1000 % 60;

    hour1 = floor(hours / 10);
    hour2 = hours % 10;
    minute1 = floor(minutes / 10);
    minute2 = minutes % 10;
    second1 = floor(seconds / 10);
    second2 = seconds % 10;
  }
  
  if (blink && modeChrono == CHRONO_PAUSED) {
    setTimeString(timeFormatted, "xx : xx", "xx:xx xx", 0, 0, 0, 0, 0, 0);
  } else {
    setTimeString(timeFormatted, "%d%d : %d%d", "%d%d:%d%d %d%d", hour1, hour2, minute1, minute2, second1, second2);
  }

  lmd.clear();
  
  displayTimeString(timeFormatted, 0);

  lmd.display();
}

void displayTimer() {
  static bool blink = false;
  static unsigned long previousMillis = 0;

  char timeFormatted[10];
  
  unsigned long currentMillis = millis();
  unsigned long delta;
  unsigned int hours = 0, hour1 = 0, hour2 = 0, minutes = 0,
    minute1 = 0, minute2 = 0, seconds = 0, second1 = 0, second2 = 0;

  if (currentMillis - previousMillis > 500) {
    previousMillis = currentMillis;
    blink = !blink;
  }
  
  switch(modeTimer) {
    case TIMER_PAUSED:
      delta = timer - ((currentMillis / 1000.0) - (timerStart / 1000.0)) + ((currentMillis / 1000.0) - (timerPause / 1000.0));
       
      hours = delta / 60 / 60;
      minutes = delta / 60 % 60;
      seconds = delta % 60;

      hour1 = floor(hours / 10);
      hour2 = hours % 10;
      minute1 = floor(minutes / 10);
      minute2 = minutes % 10;
      second1 = floor(seconds / 10);
      second2 = seconds % 10;
      
      if (blink) {
        setTimeString(timeFormatted, "xx : xx", "xx:xx xx", 0, 0, 0, 0, 0, 0);
      } else {
        setTimeString(timeFormatted, "%d%d : %d%d", "%d%d:%d%d %d%d", hour1, hour2, minute1, minute2, second1, second2);
      }
      
      break;
    case TIMER_RUNNING:
      delta = timer - ((currentMillis / 1000.0) - (timerStart / 1000.0));

      if (delta <= 0) {
        modeTimer = TIMER_OVER;
        buzzerFrequency = 1000;
        buzzerStepper = 1000;
        pinMode(BUZZER, OUTPUT);
      } else {
        hours = delta / 60 / 60;
        minutes = delta / 60 % 60;
        seconds = delta % 60;

        hour1 = floor(hours / 10);
        hour2 = hours % 10;
        minute1 = floor(minutes / 10);
        minute2 = minutes % 10;
        second1 = floor(seconds / 10);
        second2 = seconds % 10;

        setTimeString(timeFormatted, "%d%d : %d%d", "%d%d:%d%d %d%d", hour1, hour2, minute1, minute2, second1, second2);
      }
              
      break;
    case TIMER_OVER:
      if (blink) {
        setTimeString(timeFormatted, "xx : xx", "xx:xx xx", 0, 0, 0, 0, 0, 0);
      } else {
        setTimeString(timeFormatted, "00 : 00", "00:00 00", 0, 0, 0, 0, 0, 0);
      }

      buzz();
      
      break;
    default:
      if (blink) {
        switch(modeTimer) {
          case TIMER_HOUR1:
            sprintf(timeFormatted, "x%do%d%d %d%d", savedHour2, savedMinute1, savedMinute2, savedSecond1, savedSecond2);
            break;
          case TIMER_HOUR2:
            sprintf(timeFormatted, "%dxo%d%d %d%d", savedHour1, savedMinute1, savedMinute2, savedSecond1, savedSecond2);
            break;
          case TIMER_MINUTE1:
            setTimeString(timeFormatted, "x%d : %d%d", "%d%dox%d %d%d", savedHour1, savedHour2, savedMinute2, savedSecond1, savedSecond2, 0);
            break;
          case TIMER_MINUTE2:
            setTimeString(timeFormatted, "%dx : %d%d", "%d%do%dx %d%d", savedHour1, savedHour2, savedMinute1, savedSecond1, savedSecond2, 0);
            break;
          case TIMER_SECOND1:
            setTimeString(timeFormatted, "%d%d : x%d", "%d%do%d%d x%d", savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond2, 0);
            break;
          case TIMER_SECOND2:
            setTimeString(timeFormatted, "%d%d : %dx", "%d%do%d%d %dx", savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond1, 0);
            break;
        }
      } else {
        setTimeString(timeFormatted, "%d%d : %d%d", "%d%d:%d%d %d%d", savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond1, savedSecond2);
      }
  }

  lmd.clear();
  
  displayTimeString(timeFormatted, 0);

  lmd.display();
}

void displayTimeString(char *text, int cursor) {
  unsigned int len = strlen(text);
  byte letter[8];

  for(int idx = 0; idx < len; idx ++) {
    int c = text[idx] - 32;

    if (c == 0) { // SPACE
      cursor += 1;
    } else if (c == 79) { // o
      cursor += 2;
    } else if (c == 88) { // x
      cursor += 5;
    } else if (c == 26) { // :
      memcpy_P(&letter, &alphabetInProgmem[c], ROWS);
      drawSprite(letter, cursor, 1);
      cursor += 2;
    } else {
      memcpy_P(&letter, &alphabetInProgmem[c], ROWS);
      drawSprite(letter, cursor, 4);
      cursor += 5;
    }
  }
}

void setTimeString(
  char *dest,
  char *shortPattern, char *fullPattern,
  unsigned int hour1, unsigned int hour2,
  unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4) {
  if (NUM_DEVICES < 4) {
    sprintf(dest, shortPattern, val1, val2, val3, val4);
  } else {
    sprintf(dest, fullPattern, hour1, hour2, val1, val2, val3, val4);
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

