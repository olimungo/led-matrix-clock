void displaySetup() { 
  RTC_TIME rtcTime;
  
  switch(setUp.state) {
    case STATE_SETUP::SS_INTRO:
      getRtcTime(&rtcTime);
      displayTitleClock("%d%d:%d%d", &rtcTime, NO_SECONDS_BAR);
      setUp.hour1 = rtcTime.hour1;
      setUp.hour2 = rtcTime.hour2;
      setUp.minute1 = rtcTime.minute1;
      setUp.minute2 = rtcTime.minute2;
      
      setUp.state++;
      
      break;
    case STATE_SETUP::SS_SET:
      setClock();
      
      break;
  }
}

void setClock() {
  uint8_t nextBuffer[MAX_COLS], width;
  char clock[50];
  bool evenSecond = (millis() / 500) % 2 == 0;

  if (evenSecond) {
    switch(setUp.stateSelect) {
      case STATE_SETUP_SELECT::SSS_HOUR1:
        sprintf(clock, "     %d:%d%d", setUp.hour2, setUp.minute1, setUp.minute2);
        break;
      case STATE_SETUP_SELECT::SSS_HOUR2:
        sprintf(clock, "%d     :%d%d", setUp.hour1, setUp.minute1, setUp.minute2);
        break;
      case STATE_SETUP_SELECT::SSS_MINUTE1:
        sprintf(clock, "%d%d:     %d", setUp.hour1, setUp.hour2, setUp.minute2);
        break;
      case STATE_SETUP_SELECT::SSS_MINUTE2:
        sprintf(clock, "%d%d:%d     ", setUp.hour1, setUp.hour2, setUp.minute1);
        break;
    }
  } else {
    sprintf(clock, "%d%d:%d%d", setUp.hour1, setUp.hour2, setUp.minute1, setUp.minute2);
  }

  width = createBuffer(clock, nextBuffer);
  centerBuffer(nextBuffer, width);
  mx.setBuffer(MAX_COLS - 1, MAX_COLS, nextBuffer);
}
