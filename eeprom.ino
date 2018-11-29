int getTimeFormat() {  
  getConfig(&config);

  return config.timeFormat;
}

void putTimeFormat(int timeFormat) {  
  getConfig(&config);
  config.timeFormat = timeFormat;
  putConfig(&config);
}

unsigned long getTimer() {  
  getConfig(&config);
  return config.timer;
}

void putTimer(unsigned long timer) {  
  getConfig(&config);
  config.timer = timer;
  putConfig(&config);
}

void getConfig(Config *config) {  
  EEPROM.get(0, *config);
}

void putConfig(Config *config) {  
  EEPROM.put(0, *config);
}

