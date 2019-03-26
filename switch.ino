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

  pinMode(BUZZER, INPUT);
  
  switch(globalState + 1) {
    case STATE::TIMER_1:
      timer.state = STATE_TIMER::ST_INTRO;
      timer.targetTime = 0;
      displayTitle("TMR1");
      
      break;
    case STATE::TIMER_2:
      timer.state = STATE_TIMER::ST_INTRO;
      timer.targetTime = 0;
      timer.stateSelect = STATE_TIMER_SELECT::STS_HOUR1;
      displayTitle("TMR2");
      
      break;
    case STATE::CHRONO:
      chrono.state = STATE_CHRONO::SC_INTRO;
      chrono.referenceTime = 0;
      chrono.referencePausedTime = 0;
      displayTitle("CHR");
      
      break;
    case STATE::SETUP:
      setUp.state = STATE_SETUP::SS_INTRO;
      displayTitle("STP");
      
      break;
    default:
      displayTitleClock();
      
      break;
  }

  PAUSE_DISPLAY_REFERENCE_TIME = millis();

  globalState++;

  if(globalState > STATE::SETUP) {
    globalState = STATE::CLOCK;
    resetRolls(now);
  }
}

void secondarySwitchClicked() {
  pinMode(BUZZER, INPUT);
  
  switch(globalState) {
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
            case STATE_TIMER_SELECT::STS_HOUR1:
              timer.hour1++;
    
              if(timer.hour1 > 9) {
                timer.hour1 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::STS_HOUR2:
              timer.hour2++;
    
              if(timer.hour2 > 9) {
                timer.hour2 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::STS_MINUTE1:
              timer.minute1++;
    
              if(timer.minute1 > 5) {
                timer.minute1 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::STS_MINUTE2:
              timer.minute2++;
    
              if(timer.minute2 > 9) {
                timer.minute2 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::STS_SECOND1:
              timer.second1++;
    
              if(timer.second1 > 5) {
                timer.second1 = 0;
              }
              
              break;
            case STATE_TIMER_SELECT::STS_SECOND2:
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
        case STATE_TIMER::ST_END:
          timer.state = STATE_TIMER::ST_INTRO;
          timer.targetTime = 0;
          timer.stateSelect = STATE_TIMER_SELECT::STS_HOUR1;
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
    case STATE::SETUP:
      switch(setUp.state) {
        case STATE_SETUP::SS_SET:
          switch(setUp.stateSelect) {
            case STATE_SETUP_SELECT::SSS_HOUR1:
              setUp.hour1++;
    
              if(setUp.hour1 > 2) {
                setUp.hour1 = 0;
              } else if (setUp.hour1 == 2 && setUp.hour2 > 3) {
                setUp.hour2 = 0;
              }
              
              break;
            case STATE_SETUP_SELECT::SSS_HOUR2:
              setUp.hour2++;
    
              if(setUp.hour2 > 3) {
                if(setUp.hour1 == 2) {
                  setUp.hour2 = 0;
                }
              } else if(setUp.hour2 > 9) {
                setUp.hour2 = 0;
              }
              
              break;
            case STATE_SETUP_SELECT::SSS_MINUTE1:
              setUp.minute1++;
    
              if(setUp.minute1 > 5) {
                setUp.minute1 = 0;
              }
              
              break;
            case STATE_SETUP_SELECT::SSS_MINUTE2:
              setUp.minute2++;
    
              if(setUp.minute2 > 9) {
                setUp.minute2 = 0;
              }
              
              break;
          }
                    
          break;
      }
      
      break;
  }
}

void ternarySwitchClicked() {
  uint32_t now = millis();
  uint8_t nextBuffer[MAX_COLS], hour, minute;
  char clock[50];

  pinMode(BUZZER, INPUT);
  
  switch(globalState) {
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
          if(timer.stateSelect < STATE_TIMER_SELECT::STS_SECOND2) {
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
        case STATE_TIMER::ST_END:
          timer.state = STATE_TIMER::ST_INTRO;
          timer.targetTime = 0;
          timer.stateSelect = STATE_TIMER_SELECT::STS_HOUR1;
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
    case STATE::SETUP:
      switch(setUp.state) {
        case STATE_SETUP::SS_SET:
          if(setUp.stateSelect < STATE_SETUP_SELECT::SSS_MINUTE2) {
            setUp.stateSelect++;
          } else {
            hour = int(setUp.hour1) * 10 + int(setUp.hour2);
            minute = int(setUp.minute1) * 10 + int(setUp.minute2);
            
            setRtcTime(hour, minute);

            globalState = STATE::CLOCK;
            displayTitleClock();
          }
          
          break;
      }
            
      break;
  }
}

