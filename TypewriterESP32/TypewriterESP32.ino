#include <Adafruit_MCP23X17.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "EngineeringSubNet"
#define WIFI_PASS "password"

#define EST -18000
#define EDT -14400

// Establish server
AsyncWebServer server(80);

// Establish I2C expander
Adafruit_MCP23X17 mcp;

// Establish time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Itemize 20 parameters in SMS message
enum SMS {
  TOCOUNTRY,
  TOSTATE,
  SMSMESSAGESID,
  NUMMEDIA,
  TOCITY,
  FROMZIP,
  SMSSID,
  FROMSTATE,
  SMSSTATUS,
  FROMCITY,
  BODY,
  FROMCOUNTRY,
  TO,
  USQ,
  TOZIP,
  NUMSEGMENTS,
  MESSAGESID,
  ACCOUNTSID,
  FROM,
  APIVERSION
};

const long utcOffsetInSeconds = EST;

// Special keyboard characters that require a shift.
const char odds_char[] = "!@#$%&*()_+:\"?";
const char keys_char[] = "abcdefghijklmnopqrstuvwxyz.,-=;'1234567890";
const char caps_char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char ctrs_char[] = "\r, \b, \n";

uint8_t letters[128][2] = {
  { 0, 0 },  //0 NULL
  { 0, 0 },  //1 NULL
  { 0, 0 },  //2 NULL
  { 0, 0 },  //3 NULL
  { 0, 0 },  //4 NULL

  { 0, 0 },  //5 NULL
  { 0, 0 },  //6 NULL
  { 0, 0 },  //7 NULL
  { 5, 2 },  //8 BACK SPACE
  { 3, 7 },  //9 TAB

  { 0, 0 },  //10 LF
  { 0, 0 },  //11 NULL
  { 0, 0 },  //12 NULL
  { 3, 0 },  //13 CARRIAGE RETURN
  { 5, 9 },  //14 MARGIN RELEASE
  { 6, 8 },  //15 LEFT MARGIN
  { 0, 0 },  //16 NULL
  { 0, 0 },  //17 NULL
  { 0, 0 },  //18 NULL
  { 0, 0 },  //19 NULL

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

  { 0, 0 },  //30 NULL
  { 0, 0 },  //31 NULL
  { 0, 3 },  //32 SPACE
  { 6, 9 },  //33 !
  { 1, 2 },  //34 "
  { 5, 7 },  //35 #
  { 6, 7 },  //36 $
  { 6, 6 },  //37 %
  { 6, 5 },  //38 &
  { 1, 2 },  //39 '

  { 5, 4 },  //40 (
  { 5, 3 },  //41 )
  { 6, 4 },  //42 *
  { 6, 2 },  //43 +
  { 1, 3 },  //44 ,
  { 6, 3 },  //45 -
  { 0, 4 },  //46 .
  { 0, 0 },  //47 /
  { 5, 3 },  //48 0
  { 6, 9 },  //49 1

  { 5, 8 },  //50 2
  { 5, 7 },  //51 3
  { 6, 7 },  //52 4
  { 6, 6 },  //53 5
  { 5, 5 },  //54 6
  { 6, 5 },  //55 7
  { 6, 4 },  //56 8
  { 5, 4 },  //57 9
  { 1, 0 },  //58 :
  { 1, 0 },  //59 ;

  { 0, 0 },  //60 <
  { 6, 2 },  //61 =
  { 6, 2 },  //62 >
  { 0, 0 },  //63 ?
  { 5, 8 },  //64 @
  { 0, 0 },  //65 NULL
  { 0, 0 },  //66 NULL
  { 0, 0 },  //67 NULL
  { 0, 0 },  //68 NULL
  { 0, 0 },  //69 NULL

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

  { 0, 0 },  //90 NULL
  { 0, 0 },  //91 [
  { 0, 0 },  //92 NULL
  { 0, 0 },  //93 NULL
  { 0, 0 },  //94 NULL
  { 6, 3 },  //95 _
  { 0, 0 },  //96 NULL
  { 1, 7 },  //97 a
  { 1, 1 },  //98 b
  { 0, 8 },  //99 c

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

String body;
String phone;
const char *pathToFile = "/phones.txt";
bool msg = false;

String findCaller(fs::FS &fs, String n) {
  String name;
  File file = fs.open(pathToFile, "r");

  if (!file) {
    Serial.println("No Saved Data!");
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    if (line.indexOf(n) != -1) {
      name = line.substring(line.indexOf(',') + 1);
      name.trim();
    }
  }
  file.close();
  return name;
}

void addCaller(fs::FS &fs, String number) {

  File file = fs.open(pathToFile, "a");
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  number = '\n' + number + ", UNKNOWN" + '\n';
  file.print(number);
  file.close();
}

// Send one character to the typewriter
void send_character(uint8_t c) {

  bool shift;

  // Check to see which sort of character is being sent.
  char *odds = strchr(odds_char, c);
  char *keys = strchr(keys_char, c);
  char *caps = strchr(caps_char, c);
  char *ctrs = strchr(ctrs_char, c);

  if (caps != NULL) {
    shift = true;
    c += 32;
  } else if (odds != NULL) {
    shift = true;
  } else {
    shift = false;
  }
  // Serial.println(c);
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

  // Select channel on demultiplexer from which to write signal
  mcp.writeGPIOA(rx_pin_select);

  // Select channel on multiplexer from which to read signal
  mcp.writeGPIOB(tx_pin_select);

  // Turn off shift (set on by default with write to GPIOB)
  if (shift != true) mcp.digitalWrite(15, HIGH);

  // Strobe demulitplexer to type character
  mcp.digitalWrite(11, LOW);
  delay(100);
  mcp.digitalWrite(11, HIGH);
}

void setup() {
  Serial.begin(115200);
  delay(4000);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.setTimeOffset(utcOffsetInSeconds);

  server.on("/sms", HTTP_GET, [](AsyncWebServerRequest *request) {
    enum SMS data;
    const AsyncWebParameter *p;

    data = BODY;
    p = request->getParam(data);
    body = p->value();
    Serial.println(body);

    data = FROM;
    p = request->getParam(data);
    phone = p->value();
    Serial.println(phone);

    msg = true;
  });

  // Start server
  server.begin();

  // Start time
  timeClient.begin();

  // Start I2C
  if (!mcp.begin_I2C()) {
    Serial.println("No I2C.");
    return;
  }

  // Start file system
  if (!LittleFS.begin()) {
    Serial.println("No FS.");
    return;
  }

  Serial.println("All Systems Go!");

  // Set all pins on I2C expander to high
  for (int x = 0; x < 16; x++) {
    mcp.pinMode(x, OUTPUT);
    mcp.digitalWrite(x, HIGH);
  }
}

void loop() {

  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  if (msg == true) {

    // Get date and time
    String formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    String date = formattedDate.substring(2, splitT);
    String time = formattedDate.substring(splitT + 1, formattedDate.length() - 4);

    // Get name of sender or use phone number
    String id = findCaller(LittleFS, phone);
    if (id == NULL) {
      addCaller(LittleFS, phone);
      id = phone;
    }

    // Build message
    String message = date + " " + time + " " + id + ": " + body + '\r';

    // Send message
    for (int x = 0; x < (message.length()); x++) {
      send_character(message[x]);
    }
    msg = false;
  }

/* Check this every 24hours
  if (((month > MAR) && (month < NOV)) || ((month == MAR) && (previousSunday >= MAGIC_NUMBER)) || ((month == MAR) && (day > WEEK * 2)) || ((month == NOV) && (previousSunday < 1)))
    timeClient.setTimeOffset(EDT);
  else
    timeClient.setTimeOffset(EST);
    */
}
