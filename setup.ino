void checkButton() {
  if (readSwitch(MAIN_SWITCH_PIN, &mainSwitchPressed)) {
    mode++;

    if (mode > MODE_SETUP_MINUTE2) {
      mode = MODE_CLOCK;
      setRtcClock();
    } else if (mode == MODE_SETUP_HOUR1) {
      savedHour1 = floor(getHour() / 10);
      savedHour2 = getHour() % 10;
      savedMinute1 = floor(getMinute() / 10);
      savedMinute2 = getMinute() % 10;
    }
  }

  if (readSwitch(SECONDARY_SWITCH_PIN, &secondarySwitchPressed)) {
    switch(mode) {
      case MODE_SETUP_HOUR1:
        savedHour1++;

        if (savedHour1 > 2) {
          savedHour1 = 0;
        }

        if (savedHour1 == 2 && savedHour2 > 3) {
          savedHour2 = 0;
        }
        
        break;

      case MODE_SETUP_HOUR2:
        savedHour2++;

        if (savedHour1 == 2 && savedHour2 > 3) {
          savedHour1 = 0;
          savedHour2 = 0;
        } else if (savedHour2 > 9) {
          savedHour2 = 0;
        }
        
        break;
      case MODE_SETUP_MINUTE1:
        savedMinute1++;

        if (savedMinute1 > 5) {
          savedMinute1 = 0;
        }
        
        break;
      case MODE_SETUP_MINUTE2:
        savedMinute2++;

        if (savedMinute2 > 9) {
          savedMinute2 = 0;
        }
        
        break;
    }
        
  }
}

void setupMode() {
  static bool isNumberHidden = false;
  static unsigned long previousMillis = 0;
  
  unsigned long currentMillis = millis();
  char timeFormatted[10];

  if (currentMillis - previousMillis > 500) {
    previousMillis = currentMillis;
    isNumberHidden = !isNumberHidden;
  }
  
  if (isNumberHidden) {
    switch(mode) {
      case MODE_SETUP_HOUR1:
        snprintf(timeFormatted, sizeof(timeFormatted), "x%d %d%d", savedHour2, savedMinute1, savedMinute2);
        break;
      case MODE_SETUP_HOUR2:
        snprintf(timeFormatted, sizeof(timeFormatted), "%dx %d%d", savedHour1, savedMinute1, savedMinute2);
        break;
      case MODE_SETUP_MINUTE1:
        snprintf(timeFormatted, sizeof(timeFormatted), "%d%d x%d", savedHour1, savedHour2, savedMinute2);
        break;
      case MODE_SETUP_MINUTE2:
        snprintf(timeFormatted, sizeof(timeFormatted), "%d%d %dx", savedHour1, savedHour2, savedMinute1);
        break;
    }
  } else {
    snprintf(timeFormatted, sizeof(timeFormatted), "%d%d %d%d", savedHour1, savedHour2, savedMinute1, savedMinute2);
  }

  lmd.clear();
  
  drawString(timeFormatted);

  lmd.display();
}

bool readSwitch(int pin, bool *pressed) {
  int sensorValue = digitalRead(pin);

  if (sensorValue == LOW) {
    if (*pressed) {
      *pressed = false;
      return true;
    }
  } else {
    *pressed = true;
  }

  return false;
}

