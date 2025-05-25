#include <Arduino.h>

const int trigPin = 12;
const int echoPin = 14;

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration * 0.0343 / 2.0;

  distanceInch = distanceCm * 0.393701;

  Serial.print("Distance:");
  Serial.print(distanceCm);
  Serial.print("(cm) ");
  Serial.print(distanceInch);
  Serial.println("(inch)");

  delay(1000);
}