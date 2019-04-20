void buzz() {
  tone(BUZZER, buzzerFrequency, 50);

  buzzerFrequency += buzzerStepper;

  if (buzzerFrequency > 11000 || buzzerFrequency <= 1000) {
    buzzerStepper = -buzzerStepper;
  }

  // Without this delay, buzzer sounds awful
  delay(1);
}
