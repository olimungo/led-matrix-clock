int getTimeFormat() {  
  Config *config = getConfig();

  return config->timeFormat;
}

void putTimeFormat(int timeFormat) {  
  Config *config = getConfig();
  config->timeFormat = timeFormat;
  putConfig(config);
}

unsigned long getTimer() {  
  Config *config = getConfig();

  return config->timer;
}

void putTimer(unsigned long timer) {  
  Config *config = getConfig();
  config->timer = timer;
  putConfig(config);
}

Config *getConfig() {
  Config config;
  
  EEPROM.get(0, config);

  return &config;
}

void putConfig(Config *config) {  
  EEPROM.put(0, *config);
}

