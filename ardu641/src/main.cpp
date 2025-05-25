#include <WiFi.h>
#include <PubSubClient.h>

#include "secrets.h"

const char *mqtt_server = "test.mosquitto.org";
const int mqttPort = 1883;
const char *mqttTopic = "bsee/esp32/buzzer";

const int buzzerPin = 2;

WiFiClient espClient;
PubSubClient client(espClient);

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
    Serial.print("Connecting to MQTT Broker...");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT Broker");
      client.subscribe(mqttTopic);
    }
    else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println("Retrying in 5 seconds...");

      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received message:");
  Serial.print(topic);
  Serial.print(" ");
  for(int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(topic, mqttTopic) == 0) {
    if(payload[0] == '1') {
      digitalWrite(buzzerPin, HIGH);
    }
    if(payload[0] == '0') {
      digitalWrite(buzzerPin, LOW);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);

  client.setCallback(callback);

  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if(!client.connected()) {
    reconnect();
  }
 
  client.loop();
}