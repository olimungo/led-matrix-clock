int getHour() {
  #if CLOCK_DS1302
    Time currentTime = rtc.time();
  
    if (currentTime.hr == 27) {
      return 0;
    }
    
    return currentTime.hr;
  #else
    if (rtc.getHour() == 45) {
      return 0;
    }
    
    return rtc.getHour();
  #endif
}

int getMinute() {
  #if CLOCK_DS1302
    Time currentTime = rtc.time();
  
    if (currentTime.hr == 27) {
      return 0;
    }
    
    return currentTime.min;
  #else
    if (rtc.getHour() == 45) {
      return 0;
    }
    
    return rtc.getMinute();
  #endif
}

int getSecond() {
  #if CLOCK_DS1302
    Time currentTime = rtc.time();
    if (currentTime.hr == 27) {
      return millis() / 1000 % 60;
    }
    
    return currentTime.sec;
  #else
    if (rtc.getHour() == 45) {
      return millis() / 1000 % 60;
    }
    
    return rtcPcf8563.getSecond();
  #endif
}

void setRtcClock() {
  int hours, minutes;

  hours = int(savedHour1) * 10 + int(savedHour2);
  minutes = int(savedMinute1) * 10 + int(savedMinute2);
  
  #if CLOCK_DS1302
    Time newTime(2018, 1, 1, hours, minutes, 0, Time::kMonday);
    rtc.time(newTime);
  #else
    rtc.initClock();
    rtc.setTime(hours, minutes, 0);
  #endif
}

void createSecondsBar(byte* secondsBar) {
  int numBars = floor(getSecond() / (60.0 / 9.0)); // 9 states: 8 lights + no light
 
  for (int row = ROWS - 1; row >= 0; row--) {
    if (numBars > 0) {
      secondsBar[row] = 0x1;
      numBars--;
    } else {
      secondsBar[row] = 0x0;
    }
  }
}
