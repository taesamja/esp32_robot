#include <DHT.h>

#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(2000);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("온도: ");
  Serial.print(temperature);
  Serial.print(" °C, 습도: ");
  Serial.print(humidity);
  Serial.println(" %");
}