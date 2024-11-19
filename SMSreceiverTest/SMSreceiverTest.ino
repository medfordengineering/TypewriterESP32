
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
    //request->send(LittleFS, "/index.html", "text/html");
    //request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
}