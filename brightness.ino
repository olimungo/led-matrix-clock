void brightnessOneMore() {
  if (brightness.value < 10) {
    brightness.value += 1;
  }

  setBrightness(brightness.value);
  
  sendBrightnessReply();
}

void brightnessOneLess() {
  if (brightness.value > 0) {
    brightness.value -= 1;
  }

  setBrightness(brightness.value);

  sendBrightnessReply();
}

void sendBrightnessReply() {
  char result[200];

  sprintf(result, "{ \"brightness\": \"%d\" }", brightness.value);
    
  server.send(200, "application/json", result);
}
