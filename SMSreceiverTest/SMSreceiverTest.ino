/* PARAMS
0:ToCountry
1:ToState
2:SmsMessageSid
3:NumMedia
4:ToCity
5:FromZip
6:SmsSid
7:FromState
8:SmsStatus
9:FromCity
10:Body
11:FromCountry
12:To
13:�A�?
14:ToZip
15:NumSegments
16:MessageSid
17:AccountSid
18:From
19:ApiVersion

STEP 1: Create a reverse proxy with ngrok
Run ngrok on the local network using ngrok http 192.168.0.18:80 (I have run this as administrator)

STEP 2: Paste the proxy address into the webhook section of Twilio
*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>

#define WIFI_SSID "EngineeringSubNet"
#define WIFI_PASS "password"

// Set web server port number to 80
AsyncWebServer server(80);

// Variable to store the HTTP request
//String header;

//const char* PARAM_MESSAGE = "message";

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
/*
  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("html");
    request->send(LittleFS, "/index.html", "text/html");
    //request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("css");
    request->send(LittleFS, "/styles.css", "text/css");
  });
*/
  // Route for SMS request
  server.on("/sms", HTTP_GET, [](AsyncWebServerRequest* request) {
    //Serial.println("sms");
   // String inputr[20];
    String body;
    String phone;
    //int paramsNr = request->params();

    const AsyncWebParameter* p = request->getParam(10);
    body = p->value();
    Serial.println(body);

    p = request->getParam(18);
    phone = p->value();
    Serial.println(phone);

   // request->send(LittleFS, "/index.html", String(), false, processor);
    

/*
    for (int i = 0; i < paramsNr; i++) {
      const AsyncWebParameter* p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      Serial.print("Param value: ");
      Serial.print(p->value());
     // inputr[i] = p->name();
     // inputr[i] = p->value();

     // Serial.println(i);
    }
    // Serial.printf("%s:%s:%s:%s\n", inputr[3], inputr[4], inputr[10], inputr[11]);

    //request->send(LittleFS, "/index.html", "text/html");
    // request->send(LittleFS, "/index.html", String(), false, processor);
    */
  });

  //Start server

  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
}