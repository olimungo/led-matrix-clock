void chronoStart() {
  chrono.mode = MODE::RUN;

  sendOkReply();
}

void chronoPause() {
  chrono.mode = MODE::PAUSE;

  sendOkReply();
}

void chronoReset() {
  chrono.mode = MODE::OFF;
  chrono.ellapsed = 0;
  
  sendOkReply();
}
