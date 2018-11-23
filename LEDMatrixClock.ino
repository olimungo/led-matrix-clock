#include <LEDMatrixDriver.hpp>
#include <math.h>
#include <stdio.h>

#include <TimeLib.h>
#include <DS1302.h>
#include <Wire.h>
#include <Rtc_Pcf8563.h> // SCL - A5, SDA - A4, INT - 3

#define CLOCK_TYPE_DS1302 0
#define CLOCK_TYPE_PCF8563 1

#define ROWS 8
#define NUM_DEVICES 4

#define CLOCK_RESET 5
#define CLOCK_DATA 6
#define CLOCK_CLOCK 7

#define MATRIX_CS_PIN 9 // DIN (MOSI) : 11 - CLK: 13

#define MAIN_SWITCH_PIN 0
#define SECONDARY_SWITCH_PIN 1
#define TERNARY_SWITCH_PIN 2

#define MODE_CLOCK 0
#define MODE_SETUP_HOUR1 1
#define MODE_SETUP_HOUR2 2
#define MODE_SETUP_MINUTE1 3
#define MODE_SETUP_MINUTE2 4

int CLOCK_TYPE = CLOCK_TYPE_DS1302;

bool mainSwitchPressed = false;
bool secondarySwitchPressed = false;

LEDMatrixDriver lmd(NUM_DEVICES, MATRIX_CS_PIN);

DS1302 rtcDs1302(CLOCK_RESET, CLOCK_DATA, CLOCK_CLOCK);
Rtc_Pcf8563 rtcPcf8563;
                  
byte font[12][ROWS] = {
  { 0xf, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xf }, // 0
  { 0x3, 0x7, 0xb, 0x3, 0x3, 0x3, 0x3, 0x3 }, // 1
  { 0xe, 0x3, 0x3, 0x6, 0xc, 0xc, 0xc, 0xf }, // 2
  { 0xf, 0x3, 0x3, 0xf, 0x3, 0x3, 0x3, 0xf }, // 3
  { 0x8, 0x8, 0x9, 0xf, 0x3, 0x3, 0x3, 0x3 }, // 4
  { 0xf, 0xc, 0xc, 0xf, 0x3, 0x3, 0x3, 0xf }, // 5
  { 0x8, 0x8, 0x8, 0xf, 0xd, 0xd, 0xd, 0xf }, // 6
  { 0xf, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3 }, // 7
  { 0xf, 0xd, 0xd, 0xf, 0xd, 0xd, 0xd, 0xf }, // 8
  { 0xf, 0xb, 0xb, 0xf, 0x3, 0x3, 0x3, 0x3 }, // 9
  { 0x0, 0x0, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0 }, // :
};

int mode = MODE_CLOCK;
int savedHour1, savedHour2, savedMinute1, savedMinute2;

const int ANIM_DELAY = 50;

void setup() {
  // Serial.begin(9600);

  pinMode(MAIN_SWITCH_PIN, INPUT_PULLUP);
  pinMode(SECONDARY_SWITCH_PIN, INPUT_PULLUP);
  pinMode(TERNARY_SWITCH_PIN, INPUT_PULLUP);
  
  lmd.setEnabled(true);
  lmd.setIntensity(0);
  lmd.clear();
  lmd.display();
}

void loop() {
  /*
  Serial.print(getHour());
  Serial.print(":");
  Serial.print(getMinute());
  Serial.print(":");
  Serial.println(getSecond());
  delay(1000);
  */

  checkButton();
  
  if (mode == MODE_CLOCK) {
    displayTimeFull();
  } else {
    setupMode();
  }
  
  delay(ANIM_DELAY);
}
