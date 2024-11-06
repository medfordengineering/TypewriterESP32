#define PRESS   1
#define NOPRESS 0


uint8_t letter[128][2] = {
  { 6, 5 },  //0 NULL
  { 0, 0 },   //1 NULL
  { 0, 0 },   //2 NULL
  { 0, 0 },   //3 NULL
  { 0, 0 },   //4 NULL
  { 0, 0 },   //5 NULL
};
uint8_t i;
uint8_t state;

void setup() {
  Serial.begin(115200);

  pinMode(5, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  timeThis = millis();
  if (digitalRead(7) == false) {
    state = PRESS;
    delay(300);
  }

  switch (state) {
    case: PRESS
      digitalWrite(5, digitalRead(6));
      if (timeThis - timeLast >= 500) {
        timeLast = timeThis;
        state = NOPRESS;
      }
      break;
    case: NOPRESS
      break;
  }
}
