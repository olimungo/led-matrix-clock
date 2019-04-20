#define AP_NAME "Clock"
#define DST_SEC 0

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
};

struct CHRONO {
  MODE mode;
  uint32_t ellapsed;
};

struct TIMEZONE {
  int shift;
};

STATE state = ST_CLOCK;
TIMER timer = { MD_OFF, 0, 0 };
CHRONO chrono = { MD_OFF };
TIMEZONE timezone = { 2 };
bool isTimeSet = false;
