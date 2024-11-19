
#include <Adafruit_MCP23X17.h>

#define set_bit(var, pin) var |= 1 << (unsigned char)pin
#define clear_bit(var, pin) var &= ~(1 << (unsigned char)pin)
#define clr_bit(var, pin) clear_bit(var, pin)
#define test_bit(var, pin) ((var & (1 << (unsigned char)pin)) >> pin)
#define toggle_bit(var, pin) var ^= 1 << (unsigned char)pin

Adafruit_MCP23X17 mcp;

unsigned long timeThis;
unsigned long timeLast;

uint8_t letters[10][2] = {
  { 0, 0 },  //5
  { 1, 0 },  //u
  { 5, 0 }   //o
};

void setup() {
  Serial.begin(9600);

  Serial.println("MCP23xxx Blink Test!");

  if (!mcp.begin_I2C()) {
    Serial.println("Error.");
    while (1)
      ;
  }

  for (int x = 0; x < 8; x++) {
    mcp.pinMode(x, OUTPUT);
    mcp.digitalWrite(x, HIGH);
  }
  for (int i = 0; i < 3; i++) {
    if (test_bit(letters[2][0], i))
      mcp.digitalWrite(i, HIGH);
    else
      mcp.digitalWrite(i, LOW);
  }
}

void loop() {

  timeThis = millis();
  if (timeThis - timeLast >= 1000) {
    mcp.digitalWrite(7, LOW);
    delay(50);
    timeLast = timeThis;
  }
  mcp.digitalWrite(7, HIGH);
}
