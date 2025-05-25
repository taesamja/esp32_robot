#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "secrets.h"

String output2State = "off";

const int output2 = 2;

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="icon" href="data:,">
    
    <style>
        html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
        .button {background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor:pointer;}
        .button2 {background-color: #555555;}
    </style>
</head>
<body>
    <h1>ESP32 웹 서버</h1>    
    <p>GPIO 2 - 상태 %STATE%</p>
    %BUTTON%
</body>
</html>
)rawliteral";

String processor(const String& var) {
  if(var == "STATE") {
    return output2State;
  }

  if(var == "BUTTON") {
    String buttons = "";
    if (output2State == "off") {
      buttons += "<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>";
    } else {
      buttons += "<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>";
    }
    return buttons;
  }

  return String();
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  pinMode(output2, OUTPUT);
  digitalWrite(output2, LOW);

  Serial.print("연결 중: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi 연결됨.");
  Serial.println("IP 주소: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html, processor);
  });

  server.on("/2/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("GPIO 2 ON");
    output2State = "on";
    digitalWrite(output2, HIGH);
    request->send(200, "text/html", index_html, processor);
  });

  server.on("/2/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("GPIO 2 OFF");
    output2State = "off";
    digitalWrite(output2, LOW);
    request->send(200, "text/html", index_html, processor);
  });

  server.begin();
}

void loop() {

}