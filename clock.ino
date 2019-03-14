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

void setRtcClock() {
  int hour, minute;

  hour = int(savedHour1) * 10 + int(savedHour2);
  minute = int(savedMinute1) * 10 + int(savedMinute2);
  
  rtc.adjust(DateTime(2018, 1, 1, hour, minute, 0));
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
