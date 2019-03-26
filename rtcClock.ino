int getHour() {
  DateTime now = rtc.now();
  return now.hour();
}

int getMinute() {
  DateTime now = rtc.now();
  return now.minute();
}

int getSecond() {
  DateTime now = rtc.now();
  return now.second();
}

void setRtcTime() {
  int hour, minute;

  hour = int(savedHour1) * 10 + int(savedHour2);
  minute = int(savedMinute1) * 10 + int(savedMinute2);
  
  rtc.adjust(DateTime(2018, 1, 1, hour, minute, 0));
}

void getRtcTime(RTC_TIME *rtcTime) {
  uint8_t hour = getHour(), minute = getMinute(), second = getSecond();

  rtcTime->hour = hour;
  rtcTime->minute = minute;
  rtcTime->second = second;
  rtcTime->hour1 = floor(hour / 10);
  rtcTime->hour2 = hour % 10;
  rtcTime->minute1 = floor(minute / 10);
  rtcTime->minute2 = minute % 10;
  rtcTime->second1 = floor(second / 10);
  rtcTime->second2 = second % 10;
}

