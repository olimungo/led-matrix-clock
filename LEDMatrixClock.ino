#include <math.h>
#include <stdio.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <Wire.h>
#include <RTClib.h> // SDA: A4 - SCL: A5
#include <LEDMatrixDriver.hpp>

#include "global.h"

void setup() {
  Serial.begin(57600);

  pinMode(MAIN_SWITCH_PIN, INPUT_PULLUP);
  pinMode(SECONDARY_SWITCH_PIN, INPUT_PULLUP);
  pinMode(TERNARY_SWITCH_PIN, INPUT_PULLUP);
  
  lmd.setEnabled(true);
  lmd.setIntensity(0);
  lmd.clear();
  lmd.display();

  timeFormat = getRecordedTimeFormat();
  
  rtc.begin();
}

void loop() {
  checkButton();
  checkMidnight();

  switch(mode) {
    case MODE_CLOCK:
      displayTime();
      break;
    case MODE_SET_CLOCK:
      displaySetClock();
      break;
    case MODE_CHRONO:
      displayChrono();
      break;
    case MODE_TIMER:
      displayTimer();
      break;
    case MODE_SETUP:
      displaySetup();
      break;
    case MODE_EASTER_EGG:
      displayEasterEgg();
      break;
    case MODE_MIDNIGHT:
      displayMidnightMode();
      break;
  }

  delay(ANIM_DELAY);
}
