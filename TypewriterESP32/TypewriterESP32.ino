
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;

unsigned long timeThis;
unsigned long timeLast;

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
  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, LOW);
  mcp.digitalWrite(2, LOW);
  mcp.digitalWrite(7, HIGH);
}

void loop() {

  timeThis = millis();
  if (timeThis - timeLast >= 200) {
    mcp.digitalWrite(7, LOW);
    delay(50);
    timeLast = timeThis;
  }
  mcp.digitalWrite(7, HIGH);
}
