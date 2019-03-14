#include <MD_MAX72xx.h>
#include <SPI.h>
#include <TimeLib.h>
#include <RTClib.h> // SDA: A4 - SCL: A5

#include "global.h"
#include "font.h"

void resetMatrix(void) {
  mx.control(MD_MAX72XX::INTENSITY, 0);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  mx.clear();
}

void setup() {
  rtc.begin();
  mx.begin();
  
  resetMatrix();
  
  #if DEBUG
    Serial.begin(57600);
  #endif

  mx.setFont(_font);
}

void loop(void) {
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  printTime();
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  delay(100);
}
