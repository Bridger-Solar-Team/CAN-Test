void setupPins() {
  //Pins setup
  pinMode(PIN5, INPUT_PULLDOWN);
  pinMode(PIN6, INPUT_PULLDOWN);
  pinMode(PIN7, INPUT_PULLDOWN);
  pinMode(PIN8, INPUT_PULLDOWN);
  pinMode(PIN9, INPUT_PULLDOWN);
  pinMode(PIN10, INPUT_PULLDOWN);
  pinMode(PIN11, INPUT_PULLDOWN);
  pinMode(PIN12, INPUT_PULLDOWN);
}

void updatePins() {
  pins[5] = digitalRead(PIN5);
  pins[6] = digitalRead(PIN6);
  pins[7] = digitalRead(PIN7);
  pins[8] = digitalRead(PIN8);
  pins[9] = digitalRead(PIN9);
  pins[10] = digitalRead(PIN10);
  pins[11] = digitalRead(PIN11);
  pins[12] = digitalRead(PIN12);
}