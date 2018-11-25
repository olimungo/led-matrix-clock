int checkButton() {
  bool result = false;
  
  if (readSwitch(MAIN_SWITCH_PIN, &mainSwitchPressed)) {
    result = true;

    mode++;

    if (mode > MODE_TIMER) {
      mode = MODE_CLOCK;
    }
    
    switch(mode) {
      case MODE_CLOCK:
        scrollText(TXT_CLOCK);
        break;
      case MODE_SET_CLOCK:
        scrollText(TXT_SET_CLOCK);
        modeSetClock = SETCLOCK_HOUR1;
        savedHour1 = floor(getHour() / 10);
        savedHour2 = getHour() % 10;
        savedMinute1 = floor(getMinute() / 10);
        savedMinute2 = getMinute() % 10;
        break;
      case MODE_CHRONO:
        scrollText(TXT_CHRONO);
        chronoMillis = 0;
        chronoPause = 0;
        break;
      case MODE_TIMER:
        scrollText(TXT_TIMER);
        break;
    }
  }

  if (readSwitch(SECONDARY_SWITCH_PIN, &secondarySwitchPressed)) {
    switch(mode) {
      case MODE_SET_CLOCK:
        setClock();
        break;
      case MODE_CHRONO:
        if (modeChrono == CHRONO_PAUSED) {
          modeChrono = CHRONO_RUNNING;

          if (chronoMillis == 0) {
            chronoMillis = millis();
          } else {
            chronoMillis += millis() - chronoPause;
            chronoPause = 0;
          }
        } else {
          modeChrono = CHRONO_PAUSED;
          chronoPause = millis();
        }
        
        break;
    }
  }

  if (readSwitch(TERNARY_SWITCH_PIN, &ternarySwitchPressed)) {
    switch(mode) {
      case MODE_SET_CLOCK:
        modeSetClock++;
        
        if (modeSetClock > SETCLOCK_MINUTE2) {
          mode = MODE_CLOCK;
          setRtcClock();
        }
        break;
      case MODE_CHRONO:
        modeChrono = CHRONO_PAUSED;
        chronoMillis = 0;
        chronoPause = 0;
    }
  }

  return result;
}

bool readSwitch(int pin, bool *pressed) {
  int sensorValue = digitalRead(pin);

  if (sensorValue == HIGH) {
    if (*pressed) {
      *pressed = false;
      return true;
    }
  } else {
    *pressed = true;
  }

  return false;
}

void setClock() {
    switch(modeSetClock) {
      case SETCLOCK_HOUR1:
        savedHour1++;

        if (savedHour1 > 2) {
          savedHour1 = 0;
        }

        if (savedHour1 == 2 && savedHour2 > 3) {
          savedHour2 = 0;
        }
        
        break;
      case SETCLOCK_HOUR2:
        savedHour2++;

        if ((savedHour1 == 2 && savedHour2 > 3) || savedHour2 > 9) {
          savedHour2 = 0;
        }
        
        break;
      case SETCLOCK_MINUTE1:
        savedMinute1++;

        if (savedMinute1 > 5) {
          savedMinute1 = 0;
        }
        
        break;
      case SETCLOCK_MINUTE2:
        savedMinute2++;

        if (savedMinute2 > 9) {
          savedMinute2 = 0;
        }
        
        break;
    }
}

