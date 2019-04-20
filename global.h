#define AP_NAME "Clock"

enum MODE {
  OFF,
  RUN,
  PAUSE
};

struct TIMER {
  MODE mode;
  uint8_t minute1, minute2, second1, second2;
};

struct CHRONO {
  MODE mode;
  uint32_t ellapsed;
};

struct TIMEZONE {
  uint8_t shift;
};

TIMER timer = { MODE::OFF, 0, 0 };
CHRONO chrono = { MODE::OFF };
TIMEZONE timezone = { 2 };
