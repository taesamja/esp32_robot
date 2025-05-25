#include <WiFi.h>
#include <PubSubClient.h>

#include "secrets.h"

const int trigPin = 12;
const int echoPin = 14;

const char *mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsgTime = 0;

void setup_wifi() {
  delay(10);
  
  Serial.println();
  Serial.print("연결 중인 Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi 연결됨");
  Serial.println("IP 주소: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while(!client.connected()) {
    Serial.print("MQTT 연결 시도 중...");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    Serial.print("클라이언트ID: ");
    Serial.println(clientId);

    if(client.connect(clientId.c_str())) {
      Serial.println("연결됨");
    }
    else {
      Serial.print("실패, rc=");
      Serial.print(client.state());
      Serial.println(" 5초 후 다시 시도");

      delay(5000);
    }
  }
}

float readUltrasonicSensor() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);

  float distance = duration * 0.0343 / 2.0;

  return distance;
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
}

void loop() {
  if(!client.connected()) {
    reconnect();
  }
 
  client.loop();
  long now = millis();
  if(now-lastMsgTime > 1000) {
    lastMsgTime = now;
  }

  float sensorValue = readUltrasonicSensor();

  char sensorString[8];
  dtostrf(sensorValue, 1, 2, sensorString);

  client.publish("bsee/esp32/ultra", sensorString);
}