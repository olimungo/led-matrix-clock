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

struct ROLL {
  uint16_t frameRate;
  uint32_t referenceTime;
  int8_t currentBufferRow;
  uint8_t col;
  uint8_t currentDigit;
  uint8_t nextDigit;
  uint8_t len;
  uint8_t currentBuffer[8];
  uint8_t nextBuffer[8];
};

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, MATRIX_DIN_PIN, MATRIX_CLK_PIN, MATRIX_CS_PIN, NUM_DEVICES); // SPI
RTC_DS1307 rtc;

ROLL rollHour1 = { 500, 0, 0, 10 };
ROLL rollSecond1 = { 40, 0, 0, 8 };
ROLL rollSecond2 = { 40, 0, 0, 3 };

unsigned int savedHour1, savedHour2, savedMinute1, savedMinute2, savedSecond1, savedSecond2, buzzerFrequency;
