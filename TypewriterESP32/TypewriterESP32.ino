
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
uint8_t state = NOPRESS;
long timeThis;
long timeLast;

void setup() {
  Serial.begin(115200);

  pinMode(5, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT_PULLUP);

  pinMode(7, INPUT_PULLUP);
}

void loop() {
  timeThis = millis();
  timeLast = timeThis;
  if (digitalRead(7) == false) {
    while (timeThis - timeLast <= 100) {
    //  digitalWrite(8, digitalRead(9));
      digitalWrite(5, digitalRead(6));
    //  digitalWrite(8, digitalRead(9));

      timeThis = millis();
    }
    digitalWrite(5, HIGH);
    digitalWrite(8, HIGH);


    //  state = PRESS;
    //  delay(300);
  }/*

  switch (state) {
    case PRESS:
      digitalWrite(5, digitalRead(6));
      if (timeThis - timeLast >= 500) {
        timeLast = timeThis;
        state = NOPRESS;
      }
      break;
    case NOPRESS:
      break;
  }
  Serial.println(state);
  delay(20);*/
}
