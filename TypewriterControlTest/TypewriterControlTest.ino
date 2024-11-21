
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
  { 3, 2 }  //o
};

void send_character(uint8_t c) {
  uint8_t tx_pin_select = letters[c][0];
  uint8_t rx_pin_select = letters[c][1];

  if (rx_pin_select > 7) {
    rx_pin_select -= 8;
    rx_pin_select <<= 4;
    rx_pin_select |= 0x08;
  } else {
    rx_pin_select |= 0x80;
  }
  //tx_pin_select |= 0x08;
  mcp.writeGPIOA(rx_pin_select);
  mcp.writeGPIOB(tx_pin_select);

  mcp.digitalWrite(11, LOW);
  delay(50);
  mcp.digitalWrite(11, HIGH);
}

void setup() {
  Serial.begin(9600);

  Serial.println("MCP23xxx Blink Test!");

  if (!mcp.begin_I2C()) {
    Serial.println("Error.");
    while (1)
      ;
  }

  for (int x =
         0;
       x < 16; x++) {
    mcp.pinMode(x, OUTPUT);
    mcp.digitalWrite(x, HIGH);
  }

  // mcp.writeGPIOB(6);
  //while(1);
  //send_character(2);

  /*
  uint8_t letter = letters[2][1] << 3 | letters[2][0];
  Serial.println(letter, BIN);
  for (int i = 0; i < 6; i++) {
    // if (test_bit(letters[2][0], i))
    if (test_bit(letter, i))
      mcp.digitalWrite(i, HIGH);
    else
      mcp.digitalWrite(i, LOW);
  }*/
}

void loop() {
  send_character(2);
  delay(2000);
  /*
  timeThis = millis();
  if (timeThis - timeLast >= 1000) {
    mcp.digitalWrite(7, LOW);
    delay(50);
    timeLast = timeThis;
  }
  mcp.digitalWrite(7, HIGH);*/
}
