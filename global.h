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

#define NUM_DEVICES 4
#define MAX_COLS COL_SIZE * NUM_DEVICES

#define MATRIX_CLK_PIN 13
#define MATRIX_DIN_PIN 11 // MOSI
#define MATRIX_CS_PIN 9

#define MAIN_SWITCH_PIN 6
#define SECONDARY_SWITCH_PIN 7
#define TERNARY_SWITCH_PIN 8

#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW

enum STATE {
  CLOCK,
  TIMER_1,
  TIMER_2,
  CHRONO,
  END
};

enum CLOCK_FORMAT {
  SHORT,
  FULL
};

struct ROLL {
  uint8_t currentDigit;
  uint8_t nextDigit;
  uint8_t col;
  uint32_t referenceTime;
  uint8_t width;
  uint8_t currentBuffer[COL_SIZE];
  uint8_t nextBuffer[COL_SIZE];
  int8_t currentBufferRow;
};

struct SET_UP {
  CLOCK_FORMAT clockFormat;
};

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, MATRIX_DIN_PIN, MATRIX_CLK_PIN, MATRIX_CS_PIN, NUM_DEVICES); // SPI
RTC_DS1307 rtc;

// currentDigit property must not be between 0 and 9, and has to be positive, so the roll down is triggered at the start
// ...99 fits these rules...
// ...but 10, 69 and 666 would have fit too! ;-)
ROLL rollHour1 = { 99 };
ROLL rollHour2 = { 99 };
ROLL rollMinute1 = { 99 };
ROLL rollMinute2 = { 99 };
ROLL rollSecond1 = { 99 };
ROLL rollSecond2 = { 99 };

SET_UP setUp = { CLOCK_FORMAT::FULL };
uint32_t PAUSE_DISPLAY_REFERENCE_TIME = 0, PAUSE_DISPLAY_DURATION = 1000;

uint8_t state = STATE::CLOCK;
uint8_t savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond1, savedSecond2, buzzerFrequency;

