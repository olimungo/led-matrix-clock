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
  switch(state + 1) {
    case STATE::TIMER_1:
      state_timer = STATE_TIMER::INTRO;
      displayTitle("TMR1");
      break;
    case STATE::TIMER_2:
      state_timer = STATE_TIMER::INTRO;
      displayTitle("TMR2");
      break;
    case STATE::CHRONO:
      displayTitle("CHR");
      break;
    case STATE::END:
      displayTitleClock();
      break;
  }

  PAUSE_DISPLAY_REFERENCE_TIME = millis();

  state++;

  if(state == STATE::END) {
    state = STATE::CLOCK;
  }
}

void secondarySwitchClicked() {
  switch(state) {
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
      switch(state_timer) {
        case STATE_TIMER::SET:
          fiveMinuteCount += 5;

          if (fiveMinuteCount > 55) {
             fiveMinuteCount = 5;
          }
          break;
        case STATE_TIMER::RUN:
          break;
        case STATE_TIMER::PAUSE:
          break;
      }
      break;
    case STATE::TIMER_2:
      Serial.println("Secondary switch clicked");
      break;
    case STATE::CHRONO:
      Serial.println("Secondary switch clicked");
      break;
  }
}

void ternarySwitchClicked() {
  switch(state) {
    case STATE::CLOCK:
      Serial.println("Main switch clickedin CLOCK mode");
      break;
    case STATE::TIMER_1:
      switch(state_timer) {
        case STATE_TIMER::SET:
          state++;
          break;
        case STATE_TIMER::RUN:
          state++;
          break;
        case STATE_TIMER::PAUSE:
          state--;
          break;
      }
      break;
    case STATE::TIMER_2:
      Serial.println("Secondary switch clicked");
      break;
    case STATE::CHRONO:
      Serial.println("Secondary switch clicked");
      break;
  }
}

