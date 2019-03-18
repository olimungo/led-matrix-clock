void displayTimer() {
  uint8_t hour = getHour(), hour1 = floor(hour / 10), hour2 = hour % 10;
  uint8_t minute = getMinute(), minute1 = floor(minute / 10), minute2 = minute % 10;
  uint8_t second = getSecond(), second1 = floor(second / 10), second2 = second % 10;
  
  rollHour1.nextDigit = 0;
  rollHour2.nextDigit = 0;
  rollMinute1.nextDigit = 0;
  rollMinute2.nextDigit = 5;
  rollSecond1.nextDigit = 0;
  rollSecond2.nextDigit = 0;

  mx.update(MD_MAX72XX::OFF);
  
  rollDown(&rollHour1);
  rollDown(&rollHour2);
  blinkColon(21);
  rollDown(&rollMinute1);
  rollDown(&rollMinute2);
  rollDown(&rollSecond1);
  rollDown(&rollSecond2);

  mx.update(MD_MAX72XX::ON);
}


