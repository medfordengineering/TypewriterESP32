/*
0-4 ToCountry:ToState:SmsMessageSid:NumMedia
5-7 ToCity:FromZip:SmsSid:FromState
8-11 SmsStatus:FromCity:Body:FromCountry
12-15 To:�A�?:ToZip:NumSegments
16-19 MessageSid:AccountSid:From:ApiVersion
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
String header;

const char* PARAM_MESSAGE = "message";

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

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

  // Route for SMS request
  server.on("/sms", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("sms");
    String inputr[20];
    int paramsNr = request->params();
    Serial.println(paramsNr);

    for (int i = 0; i < paramsNr; i++) {
      const AsyncWebParameter* p = request->getParam(i);
      // Serial.print("Param name: ");
      //Serial.println(p->name());
      //Serial.print("Param value: ");
      //Serial.print(p->value());
     // inputr[i] = p->name();
      inputr[i] = p->value();

      //Serial.println(i);
    }
    Serial.printf("%s:%s:%s:%s\n", inputr[3], inputr[4], inputr[9], inputr[11]);
    //request->send(LittleFS, "/index.html", "text/html");
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  //Start server
  server.begin();
  Serial.println("HTTP server started");
}

String processor(const String& var) {

  return String();
}

void loop() {
}