
#include <Adafruit_MCP23X17.h>

#define set_bit(var, pin) var |= 1 << (unsigned char)pin
#define clear_bit(var, pin) var &= ~(1 << (unsigned char)pin)
#define clr_bit(var, pin) clear_bit(var, pin)
#define test_bit(var, pin) ((var & (1 << (unsigned char)pin)) >> pin)
#define toggle_bit(var, pin) var ^= 1 << (unsigned char)pin

Adafruit_MCP23X17 mcp;

unsigned long timeThis;
unsigned long timeLast;
/*
uint8_t letters[10][2] = {
  { 0, 0 },  //5
  { 1, 0 },  //u
  { 3, 2 }  //o
};*/

uint8_t letters[128][2] = {
  { 0, 0 },   //0 NULL
  { 0, 0 },   //1 NULL
  { 0, 0 },   //2 NULL
  { 0, 0 },   //3 NULL
  { 0, 0 },   //4 NULL
  { 0, 0 },   //5 NULL
  { 0, 0 },   //6 NULL
  { 0, 0 },   //7 NULL
  { 13, 3 },  //8 BACK SPACE
  { 13, 8 },  //9 TAB

  { 0, 0 },   //10 LF
  { 0, 0 },   //11 NULL
  { 0, 0 },   //12 NULL
  { 3, 0 },   //13 CARRIAGE RETURN
  { 13, 1 },  //14 MARGIN RELEASE
  { 13, 7 },  //15 LEFT MARGIN
  { 0, 0 },   //16 NULL
  { 0, 0 },   //17 NULL
  { 0, 0 },   //18 NULL
  { 0, 0 },   //19 NULL

  { 0, 0 },  //20 NULL
  { 0, 0 },  //21 NULL
  { 0, 0 },  //22 NULL
  { 0, 0 },  //23 NULL
  { 0, 0 },  //24 NULL
  { 0, 0 },  //25 NULL
  { 0, 0 },  //26 NULL
  { 0, 0 },  //27 NULL
  { 0, 0 },  //28 NULL
  { 0, 0 },  //29 NULL

  { 0, 0 },   //30 NULL
  { 0, 0 },   //31 NULL
  { 0, 3 },   //32 SPACE
  { 0, 0 },   //33 !
  { 1, 2 },   //34 "
  { 13, 8 },  //35 #
  { 13, 8 },  //36 $
  { 13, 9 },  //37 %
  { 13, 0 },  //38 &
  { 13, 3 },  //39 '

  { 13, 2 },  //40 (
  { 13, 6 },  //41 )
  { 13, 2 },  //42 *
  { 13, 3 },  //43 +
  { 1, 3 },   //44 ,
  { 6, 3 },   //45 -
  { 13, 2 },  //46 .
  { 13, 4 },  //47 /
  { 5, 3 },   //48 0
  { 6, 9 },   //49 1

  { 13, 7 },  //50 2
  { 13, 8 },  //51 3
  { 13, 8 },  //52 4
  { 13, 9 },  //53 5
  { 13, 0 },  //54 6
  { 13, 0 },  //55 7
  { 13, 2 },  //56 8
  { 13, 2 },  //57 9
  { 13, 4 },  //58 :
  { 13, 4 },  //59 ;

  { 0, 0 },   //60 <
  { 13, 3 },  //61 =
  { 0, 0 },   //62 >
  { 13, 4 },  //63 ?
  { 13, 7 },  //64 @
  { 0, 0 },   //65 NULL
  { 0, 0 },   //66 NULL
  { 0, 0 },   //67 NULL
  { 0, 0 },   //68 NULL
  { 0, 0 },   //69 NULL

  { 0, 0 },  //70 NULL
  { 0, 0 },  //71 NULL
  { 0, 0 },  //72 NULL
  { 0, 0 },  //73 NULL
  { 0, 0 },  //74 NULL
  { 0, 0 },  //75 NULL
  { 0, 0 },  //76 NULL
  { 0, 0 },  //77 NULL
  { 0, 0 },  //78 NULL
  { 0, 0 },  //79 NULL

  { 0, 0 },  //80 NULL
  { 0, 0 },  //81 NULL
  { 0, 0 },  //82 NULL
  { 0, 0 },  //83 NULL
  { 0, 0 },  //84 NULL
  { 0, 0 },  //85 NULL
  { 0, 0 },  //86 NULL
  { 0, 0 },  //87 NULL
  { 0, 0 },  //88 NULL
  { 0, 0 },  //89 NULL

  { 0, 0 },   //90 NULL
  { 0, 0 },   //91 [
  { 0, 0 },   //92 NULL
  { 0, 0 },   //93 NULL
  { 0, 0 },   //94 NULL
  { 13, 6 },  //95 _
  { 0, 0 },   //96 NULL
  { 1, 7 },   //97 a
  { 1, 1 },   //98 b
  { 0, 8 },   //99 c

  { 1, 8 },  //100 d
  { 3, 9 },  //101 e
  { 3, 8 },  //102 f
  { 3, 4 },  //103 g
  { 1, 4 },  //104 h
  { 3, 6 },  //105 i
  { 0, 5 },  //106 j
  { 3, 3 },  //107 k
  { 3, 2 },  //108 l
  { 1, 6 },  //109 m

  { 0, 6 },  //110 n
  { 4, 6 },  //111 o
  { 4, 4 },  //112 p
  { 4, 7 },  //113 q
  { 4, 8 },  //114 r
  { 1, 9 },  //115 s
  { 4, 5 },  //116 t
  { 5, 6 },  //117 u
  { 1, 5 },  //118 v
  { 4, 9 },  //119 w

  { 0, 9 },  //120 x
  { 3, 5 },  //121 y
  { 0, 7 },  //122 z
  { 0, 0 },  //123 NULL
  { 0, 0 },  //124 NULL
  { 0, 0 },  //125 NULL
  { 0, 0 },  //126 NULL

};

void send_character(uint8_t c) {
  uint8_t tx_pin_select = letters[c][0];
  uint8_t rx_pin_select = letters[c][1];

  /*  There are two mulitpexers to accomodate 12 bits (eight on the lower and four on the upper)
      The output of the lower mp is connected to channel 7 (a free channel) on the upper mp. To 
      address the lower mp you must select that address 7 on the upper mp. To address the upper mp 
      you need to subtract 8 and shift the bits by four to the ouptputs on the 23017 which is 
      controlling the upper mp. Both mps are by default enabled.
  */

  if (rx_pin_select > 7) {
    rx_pin_select -= 8;
    rx_pin_select <<= 4;
  } else {
    rx_pin_select |= 0x70;
  }

  Serial.println(rx_pin_select, HEX);

  // Select channel on demultiplexer from which to write signal
  mcp.writeGPIOA(rx_pin_select);

  // Select channel on multiplexer from which to read signal
  mcp.writeGPIOB(tx_pin_select);

  // Strobe demulitplexer to type character
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

  for (int x = 0; x < 16; x++) {
    mcp.pinMode(x, OUTPUT);
    mcp.digitalWrite(x, HIGH);
  }
  delay(5000);
}

void loop() {

/*
  char test[26] = "abcdefghijklmnopqrstuvwxyz";

  //char test[3] = "abc";
  for (int x = 0; x < 26; x++) {
    send_character(test[x]);
  }*/
  mcp.digitalWrite(15, LOW);  // Enable shift
  send_character('i');
  delay(2000);
  /*
  timeThis = millis();
  if (timeThis - timeLast >= 1000) {
    mcp.digitalWrite(7, LOW);
    delay(50);
    timeLast = timeThis;
  }*/
}
