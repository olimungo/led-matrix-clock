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

  if(setUp.clockFormat == CLOCK_FORMAT::SHORT) {
    setClockShort();
  } else {
    setClockFull();
  }
}

void loop(void) {
  handleSwitches();
  
  switch(state) {
    case STATE::CLOCK:
      displayClock();
      break;
    case STATE::TIMER:
      displayTimer();
      break;
    case STATE::CHRONO:
      break;
    case STATE::SETUP:
      break;
  }
}
