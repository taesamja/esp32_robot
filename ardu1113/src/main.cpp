#include <Arduino.h>

const int soilSensorPin = 34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("토양 습도 센서값:");
  Serial.println(analogRead(soilSensorPin));
  delay(1000);
}