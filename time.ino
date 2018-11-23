int getHour() {
  if (CLOCK_TYPE == CLOCK_TYPE_DS1302) {
    return getHourDs1302();
  } else {
    return getHourPcf8563();
  } 
}

int getMinute() {
  if (CLOCK_TYPE == CLOCK_TYPE_DS1302) {
    return getMinuteDs1302();
  } else {
    return getMinutePcf8563();
  } 
}

int getSecond() {
  if (CLOCK_TYPE == CLOCK_TYPE_DS1302) {
    return getSecondDs1302();
  } else {
    return getSecondPcf8563();
  } 
}

void setRtcClock() {
  int hours, minutes;

  hours = int(savedHour1) * 10 + int(savedHour2);
  minutes = int(savedMinute1) * 10 + int(savedMinute2);

  if (CLOCK_TYPE == CLOCK_TYPE_DS1302) {
   setRtcClockDs1302(hours, minutes);
  } else {
    setRtcClockPcf8563(hours, minutes);
  }
}

int getHourDs1302() {
  Time currentTime = rtcDs1302.time();

  if (currentTime.hr == 27) {
    return 0;
  }
  
  return currentTime.hr;
}

int getHourPcf8563() {
  if (rtcPcf8563.getHour() == 45) {
    return 0;
  }
  
  return rtcPcf8563.getHour();
}

int getMinuteDs1302() {
  Time currentTime = rtcDs1302.time();

  if (currentTime.hr == 27) {
    return 0;
  }
  
  return currentTime.min;
}

int getMinutePcf8563() {
  if (rtcPcf8563.getHour() == 45) {
    return 0;
  }
  
  return rtcPcf8563.getMinute();
}

int getSecondDs1302() {
  Time currentTime = rtcDs1302.time();

  if (currentTime.hr == 27) {
    return millis() / 1000;
  }
  
  return currentTime.sec;
}

int getSecondPcf8563() {
  if (rtcPcf8563.getHour() == 45) {
    return millis() / 1000;
  }
  
  return rtcPcf8563.getSecond();
}

void setRtcClockDs1302(int hours, int minutes) {
  Time newTime(2018, 1, 1, hours, minutes, 0, Time::kMonday);
  rtcDs1302.time(newTime);
}

void setRtcClockPcf8563(int hours, int minutes) {
  rtcPcf8563.initClock();
  rtcPcf8563.setTime(hours, minutes, 0);
}
