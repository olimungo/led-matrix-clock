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
    case STATE::TIMER:
      displayTitle("TMR");
      break;
    case STATE::CHRONO:
      displayTitle("CHRONO");
      break;
    case STATE::SETUP:
      displayTitle("SETUP");
      break;
    case STATE::END:
      displayTitleClock();
      break;
  }

  state++;

  if(state == STATE::END) {
    state = STATE::CLOCK;
  }
}

void secondarySwitchClicked() {
  switch(state) {
    case STATE::CLOCK:
      Serial.println("Main switch clickedin CLOCK mode");
      break;
    case STATE::TIMER:
      Serial.println("Secondary switch clicked");
      break;
    case STATE::CHRONO:
      Serial.println("Secondary switch clicked");
      break;
    case STATE::SETUP:
      Serial.println("Secondary switch clicked");
      break;
  }
}

void ternarySwitchClicked() {
  switch(state) {
    case STATE::CLOCK:
      Serial.println("Main switch clickedin CLOCK mode");
      break;
    case STATE::TIMER:
      Serial.println("Secondary switch clicked");
      break;
    case STATE::CHRONO:
      Serial.println("Secondary switch clicked");
      break;
    case STATE::SETUP:
      Serial.println("Secondary switch clicked");
      break;
  }
}

