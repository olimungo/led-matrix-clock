#include <MD_MAX72xx.h>
#include <SPI.h>
#include <TimeLib.h>
#include <RTClib.h> // SDA: A4 - SCL: A5

#include "global.h"
#include "font.h"

void setup() {
  #if DEBUG
    Serial.begin(57600);
  #endif

  rtc.begin();
  mx.begin();

  mx.control(MD_MAX72XX::INTENSITY, 0);
  mx.clear();

  mx.setFont(_font);
  
  pinMode(MAIN_SWITCH_PIN, INPUT_PULLUP);
  pinMode(SECONDARY_SWITCH_PIN, INPUT_PULLUP);
  pinMode(TERNARY_SWITCH_PIN, INPUT_PULLUP);

  // setClockShort();
  setClockFull();
}

void loop(void) {
  uint32_t now = millis();
  handleSwitches();

  if(PAUSE_DISPLAY_REFERENCE_TIME > 0) {
    if (now < (PAUSE_DISPLAY_REFERENCE_TIME + PAUSE_DISPLAY_DURATION)) {
      return;
    } else {
      PAUSE_DISPLAY_REFERENCE_TIME = 0;
    }
  }
  
  switch(state) {
    case STATE::CLOCK:
      displayClock();
      break;
    case STATE::TIMER_1 || STATE::TIMER_2:
      displayTimer();
      break;
    case STATE::CHRONO:
      break;
  }
}
