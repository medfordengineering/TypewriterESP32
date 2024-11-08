
#include <Adafruit_MCP23X17.h>

#define PRESS   1
#define NOPRESS 0

Adafruit_MCP23X17 mcp;


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

  if (!mcp.begin_I2C()) {
    Serial.println("Error.");
    while (1);
  }
  for (int x = 0; x < 8; i++) {
    mcp.pinMode(LED_PIN, INPUT_PULLUP);
  }
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
    pinMode(8, OUTPUT);
    
    while (timeThis - timeLast <= 100) {
     digitalWrite(5, mpc.digitalRead(0));
     // digitalWrite(8, digitalRead(9));
     // digitalWrite(5, digitalRead(6));
      timeThis = millis();
    }
    digitalWrite(5, HIGH);
    pinMode(8, INPUT);
/*
    delay(1000);
    timeThis = millis();
    timeLast = timeThis;
    // pinMode(8, OUTPUT);
    while (timeThis - timeLast <= 100) {
      //  digitalWrite(8, digitalRead(9));
      digitalWrite(5, digitalRead(6));
      timeThis = millis();
    }
    digitalWrite(5, HIGH);
    //  pinMode(8, INPUT);
  }*/




}
