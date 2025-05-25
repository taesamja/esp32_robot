#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

#include "secrets.h"

#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);

String readDHTTemperature() {
  float t = dht.readTemperature();
  if(isnan(t)) {
    Serial.println("DHT 센서에서 읽기 실패!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if(isnan(h)) {
    Serial.println("DHT 센서에서 읽기 실패!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
            integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr"
            crossorigin="anonymous">

    <style>
        html {
            font-family: Helvetica; 
            display: inline-block; 
            margin: 0px auto; 
            text-align: center;
        }
        
        h2 {
            font-size: 3.0rem;
        }

        p {
            font-size: 3.0rem;
        }

        .units {
            font-size: 1.2rem;
        }

        .dht-labels {
            font-size: 1.5rem;
            vertical-align: middle;
            padding-bottom: 15px;
        }
    </style>
</head>
<body>
    <h2>ESP32 DHT Server</h2>    
    <p>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
        <span class="dht-labels">온도</span>
        <span id="temperature">%TEMPERATURE%</span>
        <sup class="units">&deg;C</sup>
    </p>
    <p>
        <i class="fas fa-tint" style="color:#00add6;"></i>
        <span class="dht-labels">습도</span>
        <span id="humidity">%HUMIDITY%</span>
        <sup class="units">&percnt;</sup>
    </p>
</body>
<script>
    setInterval(function(){
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 200) {
                document.getElementById("temperature").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/temperature", true);
        xhttp.send();
    }, 2000);

    setInterval(function(){
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 200) {
                document.getElementById("humidity").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
    }, 2000);
</script>
</html>
)rawliteral";

String processor(const String& var) {
  if(var == "TEMPERATURE") {
    return readDHTTemperature();
  }

  if(var == "HUMIDITY") {
    return readDHTHumidity();
  }

  return String();
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Wi-Fi에 연결 중...");
  }

  Serial.println("");
  Serial.println("Wi-Fi 연결됨.");
  Serial.println("IP 주소: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTTemperature().c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readDHTHumidity().c_str());
  });

  server.begin();
}

void loop() {

}