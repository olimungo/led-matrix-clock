void displayTime() {
  if (timeFormat == SETUP_TIME_SHORT) {
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

  strcpy(format, "%02d:%02d %02d");
  
  snprintf(timeFormatted, sizeof(timeFormatted), format, getHour(), getMinute(), getSecond());

  lmd.clear();
  
  displayTimeString(timeFormatted, 0);

  lmd.display();
}

void displayTimeString(char* text, int cursor) {
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
      drawClockChar(letter, cursor, 1);
      cursor += 2;
    } else {
      memcpy_P(&letter, &alphabetInProgmem[c], ROWS);
      drawClockChar(letter, cursor, 4);
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
        snprintf(timeFormatted, sizeof(timeFormatted), "x%d:%d%d", savedHour2, savedMinute1, savedMinute2);
        break;
      case SETCLOCK_HOUR2:
        snprintf(timeFormatted, sizeof(timeFormatted), "%dx:%d%d", savedHour1, savedMinute1, savedMinute2);
        break;
      case SETCLOCK_MINUTE1:
        snprintf(timeFormatted, sizeof(timeFormatted), "%d%d:x%d", savedHour1, savedHour2, savedMinute2);
        break;
      case SETCLOCK_MINUTE2:
        snprintf(timeFormatted, sizeof(timeFormatted), "%d%d:%dx", savedHour1, savedHour2, savedMinute1);
        break;
    }
  } else {
    snprintf(timeFormatted, sizeof(timeFormatted), "%d%d:%d%d", savedHour1, savedHour2, savedMinute1, savedMinute2);
  }

  lmd.clear();
  
  displayTimeString(timeFormatted, 5);

  lmd.display();
}

void displayChrono() {
  static bool blink = false;
  static unsigned long previousMillis = 0;
  
  unsigned long currentMillis = millis();
  unsigned long delta = currentMillis - chronoMillis;
  
  char timeFormatted[15], format[15];
  int hour = 0, minute = 0, second = 0;

  if (currentMillis - previousMillis > 500) {
    previousMillis = currentMillis;
    blink = !blink;
  }

  if (chronoMillis > 0) {
    if (chronoPause > 0) {
      delta -= millis() - chronoPause;
    }

    hour = delta / 1000 / 60 / 60;
    minute = delta / 1000 / 60 % 60;
    second = delta / 1000 % 60;
  }

  if (blink) {
    snprintf(timeFormatted, sizeof(timeFormatted), "xx:xxoxx");
  } else {
    snprintf(timeFormatted, sizeof(timeFormatted), "%02d:%02d %02d", hour, minute, second);
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
  unsigned int hours, minutes, seconds;

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
      
      if (blink) {
        snprintf(timeFormatted, sizeof(timeFormatted), "xx:xx xx");
      } else {
        snprintf(timeFormatted, sizeof(timeFormatted), "%02d:%02d %02d", hours, minutes, seconds);
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
  
        snprintf(timeFormatted, sizeof(timeFormatted), "%02d:%02d %02d", hours, minutes, seconds);
      }
              
      break;
    case TIMER_OVER:
      if (blink) {
        snprintf(timeFormatted, sizeof(timeFormatted), "xx:xx xx");
      } else {
        snprintf(timeFormatted, sizeof(timeFormatted), "00:00 00");
      }

      buzz();
      
      break;
    default:
      if (blink) {
        switch(modeTimer) {
          case TIMER_HOUR1:
            snprintf(timeFormatted, sizeof(timeFormatted), "x%do%d%d %d%d", savedHour2, savedMinute1, savedMinute2, savedSecond1, savedSecond2);
            break;
          case TIMER_HOUR2:
            snprintf(timeFormatted, sizeof(timeFormatted), "%dxo%d%d %d%d", savedHour1, savedMinute1, savedMinute2, savedSecond1, savedSecond2);
            break;
          case TIMER_MINUTE1:
            snprintf(timeFormatted, sizeof(timeFormatted), "%d%dox%d %d%d", savedHour1, savedHour2, savedMinute2, savedSecond1, savedSecond2);
            break;
          case TIMER_MINUTE2:
            snprintf(timeFormatted, sizeof(timeFormatted), "%d%do%dx %d%d", savedHour1, savedHour2, savedMinute1, savedSecond1, savedSecond2);
            break;
          case TIMER_SECOND1:
            snprintf(timeFormatted, sizeof(timeFormatted), "%d%do%d%d x%d", savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond2);
            break;
          case TIMER_SECOND2:
            snprintf(timeFormatted, sizeof(timeFormatted), "%d%do%d%d %dx", savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond1);
            break;
        }
      } else {
        snprintf(timeFormatted, sizeof(timeFormatted), "%d%d:%d%d %d%d", savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond1, savedSecond2);
      }
  }

  lmd.clear();
  
  displayTimeString(timeFormatted, 0);

  lmd.display();
}

void buzz() {
  tone(BUZZER, buzzerFrequency, 50);

  buzzerFrequency += buzzerStepper;

  if (buzzerFrequency > 11000 || buzzerFrequency <= 1000) {
    buzzerStepper = -buzzerStepper;
  }
}

