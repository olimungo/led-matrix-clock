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

  if(setUp.clockFormat == CLOCK_FORMAT::SHORT) {
    rollHour1.col = 27;
    rollHour2.col = 22;
    rollMinute1.col = 15;
    rollMinute2.col = 10;
  }
}

void loop(void) {
  switch(state) {
    case STATE::CLOCK:
      displayClock();
      break;
    case STATE::TIMER:
      break;
    case STATE::CHRONO:
      break;
    case STATE::SETUP:
      break;
  }
}
