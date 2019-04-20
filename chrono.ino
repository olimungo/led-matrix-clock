void chronoStart() {
  chrono.mode = MD_RUN;

  sendOkReply();
}

void chronoPause() {
  chrono.mode = MD_PAUSE;

  sendOkReply();
}

void chronoReset() {
  chrono.mode = MD_OFF;
  chrono.ellapsed = 0;
  
  sendOkReply();
}
