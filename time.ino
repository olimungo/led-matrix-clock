void ntpConfigCallback() {
  settimeofday_cb(ntpTimeSet);
}

void ntpGetTime() {
  configTime(timezone.shift * 3600, DST_SEC, "0.europe.pool.ntp.org");
}

void ntpTimeSet() {
  isTimeSet = true;
}

void getTimeInfo(struct tm *timeInfo) {
  time_t now = time(nullptr);
  timeInfo = localtime(&now);
}

uint8_t getHour() {
  struct tm *timeInfo;
  time_t now = time(nullptr);
  timeInfo = localtime(&now);
  
  return timeInfo->tm_hour;
}

uint8_t getMinute() {
  struct tm *timeInfo;
  time_t now = time(nullptr);
  timeInfo = localtime(&now);;
  
  return timeInfo->tm_min;
}

uint8_t getSecond() {
  struct tm *timeInfo;
  time_t now = time(nullptr);
  timeInfo = localtime(&now);
  
  return timeInfo->tm_sec;
}
