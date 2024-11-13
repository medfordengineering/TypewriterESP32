
//#include <Adafruit_MCP23X17.h>

#define PRESS 1
#define NOPRESS 0

unsigned long thisTime, lastTime;

//Adafruit_MCP23X17 mcp;

uint8_t letter[128][2] = {
  { 6, 5 },  //0 NULL
  { 0, 0 },  //1 NULL
  { 0, 0 },  //2 NULL
  { 0, 0 },  //3 NULL
  { 0, 0 },  //4 NULL
  { 0, 0 },  //5 NULL
};
uint8_t i;
uint8_t state = NOPRESS;
long timeThis;
long timeLast;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void loop() {
  timeThis = millis();
  if (timeThis - timeLast >= 200) {
    digitalWrite(5, LOW);
    delay(50);
    timeLast = timeThis;
  }
  digitalWrite(5, HIGH);
}
