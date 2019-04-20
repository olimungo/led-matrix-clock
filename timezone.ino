void timezoneOneMore() {
  if (timezone.shift < 12) {
    timezone.shift += 1;
  }

  sendTimezoneReply();
}

void timezoneOneLess() {
  if (timezone.shift > 0) {
    timezone.shift -= 1;
  }

  sendTimezoneReply();
}

void sendTimezoneReply() {
  char result[200];

  sprintf(result, "{ \"shift\": \"%d\" }", timezone.shift);
    
  server.send(200, "application/json", result);
}
