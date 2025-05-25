#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include "secrets.h"

#define LED_PIN 2

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;
unsigned long sendDataPrevMillis = 0;

void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("연결 중인 Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi 연결됨");
  Serial.println("IP 주소: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  setup_wifi();

  config.api_key = FIREBASE_AUTH;
  config.database_url = FIREBASE_HOST;

  if(Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(LED_PIN, OUTPUT);

  if(Firebase.ready() && signupOK) {
    if(Firebase.RTDB.setInt(&firebaseData, "/ledState", 0)) {
      Serial.println("PASSED");

      Serial.print("PATH: ");
      Serial.println(firebaseData.dataPath());

      Serial.print("TYPE: ");
      Serial.println(firebaseData.dataType());

    }
    else {
      Serial.println("FAILED");
      
      Serial.print("REASON: ");
      Serial.println(firebaseData.errorReason());
    }
  }
}

void loop() {
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    int ledState = 0;
    if(Firebase.RTDB.getInt(&firebaseData, "/ledState")) {
      if(firebaseData.dataType() == "int") {
        ledState = firebaseData.intData();
        Serial.println(ledState);
        if(ledState == 1) {
          digitalWrite(LED_PIN, HIGH);
        }
        else {
          digitalWrite(LED_PIN, LOW);
        }
      }
    }
    else {
      Serial.println(firebaseData.errorReason());
    }
  }
}