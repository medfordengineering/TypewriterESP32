/* TODO
Add lights

Turn off bold and underscore at end of line. WORKING

Set up daylight savings time

Split getDate and getTime

Set up return for long strings. WORKING

Only allow known texters. WORKING

*/

/* NGROK AND TWILIO
STEP 1 : Create a reverse proxy with ngrok

(simple test)
Run ngrok on the local network using $ sudo ngrok http 192.168.0.64:80 

(running in background): 
a) Configure tunnel and authtoken on yaml file
  proto: http
  addr: 192.168.0.64:80

b) Install ngrok service  
  sudo ngrok service install --config /home/teacher/.config/ngrok/ngrok.yml

c) Start ngrok service
  sudo ngrok service start


d) Copy and paste webhook addrees
  Find webhook address under endpoints on the ngrok web interface

e) Paste webhook address into the webhook section on twilio and add /sms
Any new webhook MUST be placed in the Messageing Service SMSTypewriter NOT in the NOT under active numbers which is superceeded by the Messaging
Service. 

Problems:
a) check service with 

b) remove service with 
  sudo rm /etc/systemd/system/ngrok.service

*/

#include <Adafruit_MCP23X17.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <secrets.h>

#define EST -18000
#define EDT -14400

#define SHIFT 15
#define STROBE 14

// Key codes
#define CODE 13
#define LMAR 18
#define RMAR 19
#define MREL 20
#define BSPC 21

// Formating commands
#define AUTO_INDENT 50  //2
#define BOLD 49         //1
#define UNDERLINE 17    //MAR REL
#define CENTER 99       //C

#define CPM 100    // Characters per millisecond
#define MARGIN 88  // Total type margine

#define MAR 3
#define NOV 11

// Server object
AsyncWebServer server(80);

// I2C expander object
Adafruit_MCP23X17 mcp;

// Time objects
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// The 20 parameters in an SMS message
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
const char keys_char[] = "abcdefghijklmnopqrstuvwxyz.,-=;'1234567890/";
const char caps_char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char ctrs_char[] = "\r, \b, \n";

/* Codes for SMS characters. The columns coorespond to write pins and read pins respectively. 
The codes are a merge of GSM7 and ASCII which are both 0-127 character sets. It appears that Twilio
defaults to ASCII over GSM7, but I am still waiting to hear back from them about this.
*/
uint8_t letters[128][2] = {
  { 0, 0 },  //0 @
  { 0, 0 },  //1 £
  { 0, 0 },  //2 $ (dollar sign ?)
  { 0, 0 },  //3 ¥
  { 0, 0 },  //4 è

  { 0, 0 },  //5 é
  { 0, 0 },  //6 ù
  { 0, 0 },  //7 ì
  { 0, 0 },  //8 Ò
  { 0, 0 },  //9 ç

  { 0, 0 },  //10 LF
  { 0, 0 },  //11 Ø
  { 0, 0 },  //12 ø
  { 3, 0 },  //13 CARRIAGE RETURN
  { 0, 0 },  //14 Å
  { 6, 8 },  //15 å
  { 0, 0 },  //16 DELTA
  { 5, 9 },  //17 LOW LINE
  { 6, 8 },  //18 PHI (LEFT MARGIN)
  { 2, 8 },  //19 GAMMA (RIGHT MARGIN)

  { 5, 9 },  //20 LAMBDA (MARGIN RELEASE)
  { 5, 2 },  //21 OMEGA (BACK SPACE)
  { 0, 0 },  //22 PI
  { 0, 0 },  //23 PSI
  { 0, 0 },  //24 SIGMA
  { 0, 0 },  //25 THETA
  { 0, 0 },  //26 XI
  { 0, 0 },  //27 ESCAPE TO EXTENSION TABLE
  { 0, 0 },  //28 Æ
  { 0, 0 },  //29 æ

  { 0, 0 },  //30 ß
  { 0, 0 },  //31 É
  { 0, 3 },  //32 SPACE
  { 6, 9 },  //33 !
  { 1, 2 },  //34 "
  { 5, 7 },  //35 #
  { 6, 7 },  //36 $ (CURRANCY SIGN?)
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
  { 0, 0 },  //65 NULL (Capital letters are accessed through a software shift)
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
  { 0, 0 },  //91 Ä
  { 0, 0 },  //92 Ö
  { 0, 0 },  //93 Ñ
  { 0, 0 },  //94 Ü
  { 6, 3 },  //95 §
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

String asg = "System Ready!\r";
const char *pathToFile = "/phones.txt";

// Time variables
String date;
String timeofday;
int8_t dayofweek;
uint8_t month;
int8_t day;

bool msg = false;
bool code = false;
bool valid_user = false;
bool autoreturn = false;
bool bold = false;
bool underline = false;
bool daylightsavings = false;

uint8_t carriage_index;

// Prints out the contents of the phone file on startup
void readFile(fs::FS &fs) {
  Serial.printf("Reading file: %s\r\n", pathToFile);

  File file = fs.open(pathToFile);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

// Determines if a callers number is in the phnoe file and returns a name, UNKNOWN or NULL for not found
String findCaller(fs::FS &fs, String n) {
  String name;
  File file = fs.open(pathToFile, "r");

  if (!file) {
    Serial.println("No Saved Data!");
  }

  // Read through the file line by line. If callers number is found, return the corresponding name.
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

// Adds a callers phone number to the database with a name listed as UNKNOWN
void addCaller(fs::FS &fs, String number) {

  File file = fs.open(pathToFile, "a");
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  number = '\n' + number + ", UNKNOWN";
  file.print(number);
  file.close();
}

// Send one character to the typewriter
void send_character(uint8_t c) {
  static char last_character;
  bool shift = false;

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

  // Each character corresponds to two bytes in letter array that write and read pins respectively
  uint8_t tx_pin_select = letters[c][0];
  uint8_t rx_pin_select = letters[c][1];

  /*  There are two 8-channel mulitpexers to accomodate 12 bits. This requires 6 address bits. Three for the lower MP and
      3 for the upper MP. The output of the lower MP is connected to channel 7 (a free channel) on the upper MP. To address the 
      lower MP you must select that address for the lower byte and 7 for the upper byte to access channel 7. To address the upper MP 
      you need to subtract 8 and shift the bits by 4.
  */
  if (rx_pin_select > 7) {
    rx_pin_select -= 8;
    rx_pin_select <<= 4;
  } else {
    rx_pin_select |= 0x70;
  }

  // Select channel on demultiplexer from which to write signal
  mcp.writeGPIOA(rx_pin_select);

  // Select channel on multiplexer from which to read signal and disable strobe with OR.
  mcp.writeGPIOB(tx_pin_select | 0x40);

  // Turn on SHIFT (set off, LOW, by default with write to GPIOB) give time to settle
  if (shift == true) mcp.digitalWrite(SHIFT, HIGH);
  if (shift == true) delay(200);

  // Turn on CODE (set off, LOW, by default with write to GPIOB) give time to settle
  if (code == true) mcp.digitalWrite(CODE, HIGH);
  if (code == true) delay(200);

  // Delay to allow for double characters. The typewriter by default rejects double characters when printed too quickly
  if (last_character == c) delay(50);

  // Strobe demulitplexer to type character
  mcp.digitalWrite(STROBE, LOW);
  delay(100);
  mcp.digitalWrite(STROBE, HIGH);
  last_character = c;

  // Turn off SHIFT
  mcp.digitalWrite(SHIFT, LOW);

  // Turn off CODE
  mcp.digitalWrite(CODE, LOW);

  // Check if carriage is at the paper margin, if so, reset the carriage counter and send a return
  if (carriage_index++ > MARGIN) {
    carriage_index = 0;
    send_character('-');
    send_character('\r');
  }
}

void send_command(uint8_t c) {
  code = true;
  send_character(c);
  code = false;
}

void tprint(String s) {
  int i = 0;
  while (i < s.length()) {

    if (s[i] == '\r') carriage_index = 0;  // Reset the carriage counter each time a return is found.

    if (s[i] == '*') {
      bold = !bold;  // Set state of bold in case a user forgets the second asterix
      send_command(BOLD);
    } else if (s[i] == '_') {
      underline = !underline;  // Set the state of underline in a user forgets the second underline
      send_command(UNDERLINE);
    } else
      send_character(s[i]);
    i++;
  }
}

void setDateTime() {
  getDateTime();
  if ((month > MAR) && (month < NOV)) {
    daylightsavings = true;
  } else if ((month == MAR) && ((day - dayofweek) >= 8)) {
    daylightsavings = true;
  } else if ((month == NOV) && ((day - dayofweek) <= 1)) {
    daylightsavings = true;
  } else {
    daylightsavings = false;
  }

  if (daylightsavings == true)
    timeClient.setTimeOffset(EDT);
  else
    timeClient.setTimeOffset(EST);
}

void getDateTime() {
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  String formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  date = formattedDate.substring(2, splitT);
  timeofday = formattedDate.substring(splitT + 1, formattedDate.length() - 4);
  dayofweek = timeClient.getDay();
  month = date.substring(3, 5).toInt();
  day = date.substring(6, 8).toInt();
  // Serial.printf("date: %s\r\n", date);
  // Serial.printf("time: %s\r\n", timeofday);
  // Serial.printf("dayweek: %d and month: %d and day: %d\r\n", dayofweek, month, day);
}

void setup() {
  int i;

  Serial.begin(115200);
  delay(2000);

  // Initialize typewriter
  if (!mcp.begin_I2C()) {
    Serial.println("No I2C.");
    return;
  }

  // Set typewriter key control to neutral
  for (int x = 0; x < 16; x++) {
    mcp.pinMode(x, OUTPUT);
    mcp.digitalWrite(x, HIGH);
  }

  // Turns shift off
  mcp.digitalWrite(SHIFT, LOW);

  tprint("\r");
  send_command(CENTER);
  tprint("Panasonic T36 Enabled.\r");

  // Start WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tprint(".");
  }

  tprint("Connected to WiFi.\r");
  tprint("IP address: ");
  tprint(WiFi.localIP().toString());
  tprint("\r");

  server.on("/sms", HTTP_GET, [](AsyncWebServerRequest *request) {
    enum SMS data;
    const AsyncWebParameter *p;

    data = BODY;  // SMS message body
    p = request->getParam(data);
    body = p->value();
    Serial.println(body);

    data = FROM;  // SMS sender phone number
    p = request->getParam(data);
    phone = p->value();
    Serial.println(phone);

    msg = true;  // We have a message
  });

  // Start server
  server.begin();

  tprint("Server started.\r");

  // Start time
  timeClient.begin();

  setDateTime();

  tprint("NTP client estalished.\r");

  // Start file system
  if (!LittleFS.begin()) {
    Serial.println("No FS.");
    tprint("Error: No file system.");
    return;
  } else
    tprint("File System Initialized.\r");

  Serial.println(asg);

  // Print out phone list
  readFile(LittleFS);

  tprint(asg);
  delay(asg.length() * CPM);

  // Turn on auto-indent
  // send_command(AUTO_INDENT);
}

void loop() {



  if (msg == true) {

    // Turn off bold and underline if either were left on from previous message
    if (bold) send_command(BOLD);
    if (underline) send_command(UNDERLINE);

    //Serial.print(body);

    getDateTime();
    // Get date and time

    // Get name of sender or use phone number
    String id = findCaller(LittleFS, phone);
    if ((id == NULL) || (id == "UNKNOWN")) {
      addCaller(LittleFS, phone);
      id = phone;
      valid_user = false;
    } else
      valid_user = true;

    // Build message
    String message = date + " " + timeofday + " " + id + ": " + body + '\r';

    // Send message only if valid user
    if (valid_user == true)
      tprint(message);

    Serial.print(message);
    msg = false;
  }
}
