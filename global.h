#define DEBUG 1   // Enable or disable (default) debugging output

#if DEBUG
  #define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); } // decimal
  #define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); } // hex
  #define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); } // binary
  #define PRINTC(s, v)  { Serial.print(F(s)); Serial.print((char)v); } // char
  #define PRINTS(s)     { Serial.print(F(s)); } // string
#else
  #define PRINT(s, v)
  #define PRINTX(s, v)
  #define PRINTB(s, v)
  #define PRINTC(s, v)
  #define PRINTS(s)
#endif

#define ROWS 8
#define NUM_DEVICES 4

#define MATRIX_CLK_PIN 13
#define MATRIX_DIN_PIN 11 // MOSI
#define MATRIX_CS_PIN 9

#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW

#define ANIMATION_DELAY 75 // milliseconds
#define MAX_FRAMES 4 // number of animation frames

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, MATRIX_DIN_PIN, MATRIX_CLK_PIN, MATRIX_CS_PIN, NUM_DEVICES); // SPI
RTC_DS1307 rtc;

unsigned int savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond1, savedSecond2, buzzerFrequency;
