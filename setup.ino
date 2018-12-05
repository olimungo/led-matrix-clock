int checkButton() {
  bool result = false;
  unsigned long currentMillis = millis();
  int hours;
  int minutes;
  int seconds;

  // Check if main button was pressed for more than 3 seconds
  int sensorValue = digitalRead(MAIN_SWITCH_PIN);

  if (sensorValue == LOW) {
    if (easterEggMillis == 0) {
      easterEggMillis = currentMillis;
    }   
  }
  
  if (readSwitch(MAIN_SWITCH_PIN, &mainSwitchPressed)) {
    result = true;
    pinMode(BUZZER, INPUT);

    mode++;

    if (mode > MODE_SETUP) {
      mode = MODE_CLOCK;
    }

    if (currentMillis - easterEggMillis > 1000) {
      mode = MODE_EASTER_EGG;
      randomSeed(millis());
    }

    easterEggMillis = 0;
    
    switch(mode) {
      case MODE_CLOCK:
        scrollText(TXT_CLOCK);
        
        break;
      case MODE_SET_CLOCK:
        modeSetClock = SETCLOCK_HOUR1;
        savedHour1 = floor(getHour() / 10);
        savedHour2 = getHour() % 10;
        savedMinute1 = floor(getMinute() / 10);
        savedMinute2 = getMinute() % 10;
        
        scrollText(TXT_SET_CLOCK);
        
        break;
      case MODE_CHRONO:
        modeChrono = CHRONO_PAUSED;
        chronoMillis = 0;
        chronoPause = 0;
        
        scrollText(TXT_CHRONO);
        
        break;
      case MODE_TIMER:
        modeTimer = TIMER_HOUR1;
        timerPause = 0;
        timer = getTimer();

        hours = timer / 60 / 60;
        minutes = (timer / 60) % 60;
        seconds = timer % 60;

        savedHour1 = (hours - (hours % 10)) / 10;
        savedHour2 = hours % 10;
        savedMinute1 = (minutes - (minutes % 10)) / 10;
        savedMinute2 = minutes % 10;
        savedSecond1 = (seconds - (seconds % 10)) / 10;
        savedSecond2 = seconds % 10;
        
        scrollText(TXT_TIMER);
        
        break;
      case MODE_SETUP:
        modeSetup = SETUP_TIME_SHORT;
        
        scrollText(TXT_SETUP);
        
        break;
    }
  }

  if (readSwitch(SECONDARY_SWITCH_PIN, &secondarySwitchPressed)) {
    result = true;
    pinMode(BUZZER, INPUT);
    
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
      case MODE_TIMER:
        if (modeTimer < TIMER_PAUSED) {
          setTimer();
        } else if (modeTimer != TIMER_OVER) {
          if (modeTimer == TIMER_PAUSED) {
            modeTimer = TIMER_RUNNING;
            timerStart += millis() - timerPause;
          } else {
            modeTimer = TIMER_PAUSED;
            timerPause = millis();
          }
        }
        
        break;
      case MODE_SETUP:
        if(modeSetup == SETUP_TIME_SHORT) {
          modeSetup = SETUP_TIME_FULL;
        } else {
          modeSetup = SETUP_TIME_SHORT;
        }
        
        break;
    }
  }

  if (readSwitch(TERNARY_SWITCH_PIN, &ternarySwitchPressed)) {
    result = true;
    pinMode(BUZZER, INPUT);
    
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
        
        break;
      case MODE_TIMER:
        if (modeTimer == TIMER_PAUSED || modeTimer == TIMER_RUNNING || modeTimer == TIMER_OVER) {
            modeTimer = TIMER_HOUR1;
        } else {
          modeTimer++;
          
          if (modeTimer == TIMER_PAUSED) {
            if (savedHour1 == 0 && savedHour2 == 0 && savedMinute1 == 0 &&
              savedMinute2 == 0 && savedSecond1 == 0 && savedSecond2 == 0) {
              modeTimer = TIMER_HOUR1;
            } else {
              timer = ((savedHour1 * 10 + savedHour2) * 60 * 60) +
                ((savedMinute1 * 10 + savedMinute2) * 60) +
                (savedSecond1 * 10 + savedSecond2);
              timerStart = 0;
              timerPause = 0;
              putTimer(timer);
            }
          }
        }
        
        break;
      case MODE_SETUP:
        mode = MODE_CLOCK;
        timeFormat = modeSetup;
        putTimeFormat(timeFormat);
        break;
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

void setTimer() {
    switch(modeTimer) {
      case TIMER_HOUR1:
        savedHour1++;

        if (savedHour1 > 9) {
          savedHour1 = 0;
        }
        
        break;
      case TIMER_HOUR2:
        savedHour2++;

        if (savedHour2 > 9) {
          savedHour2 = 0;
        }
        
        break;
      case TIMER_MINUTE1:
        savedMinute1++;

        if (savedMinute1 > 5) {
          savedMinute1 = 0;
        }
        
        break;
      case TIMER_MINUTE2:
        savedMinute2++;

        if (savedMinute2 > 9) {
          savedMinute2 = 0;
        }
        
        break;
      case TIMER_SECOND1:
        savedSecond1++;

        if (savedSecond1 > 5) {
          savedSecond1 = 0;
        }
        
        break;
      case TIMER_SECOND2:
        savedSecond2++;

        if (savedSecond2 > 9) {
          savedSecond2 = 0;
        }
        
        break;
    }
}
