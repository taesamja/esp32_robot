#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include "secrets.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

String databasePath = "/room1";
String sensorPath = "/lightsensor";
String timePath = "/timestamp";

String parentPath;

int timestamp;
FirebaseJson json;

const char *ntpServer = "pool.ntp.org";

int sensorPin = 34;

unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 20000;

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

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return 0;
  }
  time(&now);

  return now;
}

void setup() {
  Serial.begin(115200);

  setup_wifi();
  configTime(0, 0, ntpServer);

  config.api_key = FIREBASE_AUTH;
  config.database_url = FIREBASE_HOST;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  if(Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  config.max_token_generation_retry = 5;
  
  Firebase.begin(&config, &auth);
}

void loop() {
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    timestamp = getTime();
    Serial.print("time:");
    Serial.println(timestamp);

    parentPath = databasePath;
    parentPath += "/";
    parentPath += String(timestamp);

    json.set(sensorPath.c_str(), String(analogRead(sensorPin)));
    json.set(timePath, String(timestamp));
    Serial.println("Set json ... ");

    if(Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json)) {
      Serial.println("ok");
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }
}