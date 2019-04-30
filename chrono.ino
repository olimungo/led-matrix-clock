void displayChrono() {
  switch(chrono.mode) {
    case MD_OFF:
      updateDisplay();     
      break;
    case MD_RUN:
      runChrono();
      break;
  }
}

void chronoStart() {
  state = ST_CHRONO;
  timer.mode= MD_OFF;
  
  if (chrono.mode == MD_OFF) {
    chrono.mode = MD_RUN;
    chrono.referenceTime = millis();
  }

  sendOkReply();
}

void chronoPause() {
  state = ST_CHRONO;
  timer.mode= MD_OFF;
  
  if (chrono.mode == MD_RUN) {
    chrono.mode = MD_PAUSE;
    chrono.referencePausedTime = millis();
  } else if ((chrono.mode == MD_PAUSE)) {
    chrono.mode = MD_RUN;
    chrono.referenceTime += millis() - chrono.referencePausedTime;
  } else {
    chronoReset();
  }

  sendOkReply();
}

void chronoReset() {  
  state = ST_CHRONO;

  chrono.mode = MD_OFF;
  timer.mode = MD_OFF;
  
  roll1.nextDigit = 0;
  roll2.nextDigit = 0;
  roll3.nextDigit = 0;
  roll4.nextDigit = 0;

  updateRollReferenceTime(millis());

  sendOkReply();
}

void runChrono() {
  uint32_t ellapsedTime = millis() - chrono.referenceTime;
  uint8_t minute, second;

  minute = ellapsedTime / 1000 / 60 % 60;
  second = ellapsedTime / 1000 % 60;

  roll1.nextDigit = floor(minute / 10);
  roll2.nextDigit = minute % 10;
  roll3.nextDigit = floor(second / 10);
  roll4.nextDigit = second % 10;

  updateDisplay();
}
