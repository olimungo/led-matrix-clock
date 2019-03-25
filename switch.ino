void handleSwitches() {
  static bool lastValueSwitch1 = false, lastValueSwitch2 = false, lastValueSwitch3 = false;
  static uint32_t lastReadSwitch1, lastReadSwitch2, lastReadSwitch3;
  
  handleSwitch(MAIN_SWITCH_PIN, &lastValueSwitch1, &lastReadSwitch1);
  handleSwitch(SECONDARY_SWITCH_PIN, &lastValueSwitch2, &lastReadSwitch2);
  handleSwitch(TERNARY_SWITCH_PIN, &lastValueSwitch3, &lastReadSwitch3);
}

void handleSwitch(uint8_t pin, bool *lastValue, uint32_t *lastRead) {
  uint8_t DEBOUNCE = 100;
  bool clicked = false;
  int currentValue = digitalRead(pin);
  uint32_t now = millis();

  if(now > *lastRead + DEBOUNCE) {
    *lastRead = now;
  } else {
    return;
  }

  if (!currentValue) {
    if(*lastValue) {
      *lastValue = false;
      clicked = true;
    }
  } else {
    *lastValue = true;
  }

  if(clicked) {
    switch(pin) {
      case MAIN_SWITCH_PIN:
        mainSwitchClicked();
        break;
      case SECONDARY_SWITCH_PIN:
        secondarySwitchClicked();
        break;
      case TERNARY_SWITCH_PIN:
        ternarySwitchClicked();
        break;
    }
  }
}

void mainSwitchClicked() {
  uint32_t now = millis();
  
  switch(setUp.state + 1) {
    case STATE::TIMER_1:
      timer.state = STATE_TIMER::ST_INTRO;
      timer.targetTime = 0;
      
      displayTitle("TMR1");
      
      break;
    case STATE::TIMER_2:
      timer.state = STATE_TIMER::ST_INTRO;
      timer.targetTime = 0;
      timer.stateSelect = STATE_TIMER_SELECT::HOUR1;
        
      displayTitle("TMR2");
      
      break;
    case STATE::CHRONO:
      chrono.state = STATE_CHRONO::SC_INTRO;
      chrono.referenceTime = 0;
      chrono.referencePausedTime = 0;
      
      displayTitle("CHR");
      
      break;
    case STATE::SETUP:
      displayTitle("STP");
      break;
    default:
      displayTitleClock();
      break;
  }

  PAUSE_DISPLAY_REFERENCE_TIME = millis();

  setUp.state++;

  if(setUp.state > STATE::SETUP) {
    setUp.state = STATE::CLOCK;
    resetRolls(now);
  }
}

void secondarySwitchClicked() {
  switch(setUp.state) {
    case STATE::CLOCK:
      if(setUp.clockFormat == CLOCK_FORMAT::SHORT) {
        setClockFull();
      } else {
        setUp.clockFormat = CLOCK_FORMAT::SHORT;
        setClockShort();
      }

      displayTitleClock();
      
      break;
    case STATE::TIMER_1:
      switch(timer.state) {
        case STATE_TIMER::ST_SET:
          timer.fiveMinuteCount += 5;

          if (timer.fiveMinuteCount > 55) {
             timer.fiveMinuteCount = 5;
          }
                    
          break;
        case STATE_TIMER::ST_RUN:
          break;
        case STATE_TIMER::ST_PAUSE:
          timer.state = STATE_TIMER::ST_INTRO;
          timer.targetTime = 0;
          break;
      }
      
      break;
    case STATE::TIMER_2:
      switch(timer.state) {
        case STATE_TIMER::ST_SET:
          switch(timer.stateSelect) {
            case STATE_TIMER_SELECT::HOUR1:
              timer.hour1++;
    
              if(timer.hour1 > 9) {
                timer.hour1 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::HOUR2:
              timer.hour2++;
    
              if(timer.hour2 > 9) {
                timer.hour2 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::MINUTE1:
              timer.minute1++;
    
              if(timer.minute1 > 5) {
                timer.minute1 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::MINUTE2:
              timer.minute2++;
    
              if(timer.minute2 > 9) {
                timer.minute2 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::SECOND1:
              timer.second1++;
    
              if(timer.second1 > 5) {
                timer.second1 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::SECOND2:
              timer.second2++;
    
              if(timer.second2 > 9) {
                timer.second2 = 0;
              }
              
              break;
          }
                    
          break;
        case STATE_TIMER::ST_RUN:
          break;
        case STATE_TIMER::ST_PAUSE:
          timer.state = STATE_TIMER::ST_INTRO;
          timer.targetTime = 0;
          timer.stateSelect = STATE_TIMER_SELECT::HOUR1;
          break;
      }
      
      break;
    case STATE::CHRONO:
      if(STATE_CHRONO::SC_PAUSE) {
        chrono.state = STATE_CHRONO::SC_INTRO;
        chrono.referenceTime = 0;
        chrono.referencePausedTime = 0;
      }
      
      break;
  }
}

void ternarySwitchClicked() {
  uint32_t now = millis();
  uint8_t nextBuffer[MAX_COLS];
  char clock[50];
  
  switch(setUp.state) {
    case STATE::CLOCK:
      break;
    case STATE::TIMER_1:
      switch(timer.state) {
        case STATE_TIMER::ST_SET:
          if(timer.targetTime == 0) {
            timer.targetTime = now + timer.fiveMinuteCount * 60.0 * 1000.0;
            sprintf(clock, "00:%02d 00", timer.fiveMinuteCount);
            createBuffer(clock, nextBuffer);
            mx.setBuffer(MAX_COLS - 1, MAX_COLS, nextBuffer);
    
            resetRolls(now);

            timer.state++;
          }

          break;
        case STATE_TIMER::ST_RUN:
          timer.referencePausedTime = millis();
          timer.state++;
          break;
        case STATE_TIMER::ST_PAUSE:
          timer.targetTime += millis() - timer.referencePausedTime;
          timer.state--;
          break;
      }
      
      break;
    case STATE::TIMER_2:
      switch(timer.state) {
        case STATE_TIMER::ST_SET:
          if(timer.stateSelect < STATE_TIMER_SELECT::SECOND2) {
            timer.stateSelect++;
          } else {
            timer.targetTime = now +
              timer.hour1 * 10.0 * 60.0 * 60.0 * 1000.0 +
              timer.hour2 * 60.0 * 60.0 * 1000.0 +
              timer.minute1 * 10.0 * 60.0 * 1000.0 +
              timer.minute2 * 60.0 * 1000.0 +
              timer.second1 * 10.0 * 1000.0 +
              timer.second2 * 1000.0;
    
            resetRolls(now);
  
            timer.state++;
          }
          
          break;
        case STATE_TIMER::ST_RUN:
          timer.referencePausedTime = millis();
          timer.state++;
          break;
        case STATE_TIMER::ST_PAUSE:
          timer.targetTime += millis() - timer.referencePausedTime;
          timer.state--;
          break;
      }
            
      break;
    case STATE::CHRONO:
      switch(chrono.state) {
        case STATE_CHRONO::SC_PAUSE:
          chrono.referenceTime += millis() - chrono.referencePausedTime;
          chrono.state++;
          
          break;
        case STATE_CHRONO::SC_RUN:
          chrono.referencePausedTime = millis();
          chrono.state--;
          
          break;
      }
      
      break;
  }
}

