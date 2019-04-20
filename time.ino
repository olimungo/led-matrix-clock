void ntpConfigCallback() {
  settimeofday_cb(ntpTimeSet);
}

void ntpGetTime() {
  configTime(timezone.shift * 3600, DST_SEC, "0.europe.pool.ntp.org", "time.nist.gov", "pool.ntp.org");
}

void ntpTimeSet() {
  isTimeSet = true;
}
