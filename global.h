#define AP_NAME "Clock"
#define DST_SEC 0

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define NUM_DEVICES 3
#define MATRIX_CS_PIN 15 // SS - SCK/CLK = 14 - MOSI/DIN = 13

#define MAX_COLS COL_SIZE * NUM_DEVICES

#define REFRESH_RATE_NTP_TIME 60 * 60 * 24

enum STATE {
  ST_CLOCK,
  ST_TIMER,
  ST_CHRONO
};

enum MODE {
  MD_OFF,
  MD_RUN,
  MD_PAUSE
};

struct TIMER {
  MODE mode;
  uint8_t minute1, minute2, second1, second2;
  uint32_t targetTime, referencePausedTime;
};

struct CHRONO {
  MODE mode;
  uint32_t referenceTime, referencePausedTime;
};

struct TIMEZONE {
  int shift;
};


struct BRIGHTNESS {
  int value;
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

ESP8266WebServer server(80);
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, MATRIX_CS_PIN, NUM_DEVICES); // SPI

STATE state = ST_CLOCK;
TIMER timer = { MD_OFF, 0, 0 };
CHRONO chrono = { MD_OFF };
TIMEZONE timezone = { 2 };
BRIGHTNESS brightness = { 0 };
Ticker timerRefreshNtpTime;
bool isTimeSet = false;

// currentDigit property must not be between 0 and 9, and has to be positive, so the roll down is triggered at the start
// ...99 fits these rules...
// ...but 10, 69 and 666 would have fit too! ;-)
ROLL roll1 = { 99 };
ROLL roll2 = { 99 };
ROLL roll3 = { 99 };
ROLL roll4 = { 99 };
