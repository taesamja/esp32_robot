// #include <WiFi.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include "time.h"  // NTP 시간 동기화를 위한 라이브러리

// // OLED 설정
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// // WiFi 설정
// #include "secrets.h"

// // NTP 서버 설정
// const char* ntpServer = "pool.ntp.org";
// const long  gmtOffset_sec = 9 * 3600;  // 한국은 UTC+9
// const int   daylightOffset_sec = 0;

// void setup() {
//   Serial.begin(115200);

//   // OLED 초기화
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("SSD1306 allocation failed");
//     while(true);
//   }
//   display.clearDisplay();
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);

//   // WiFi 연결
//   WiFi.begin(ssid, password);
//   display.println("Connecting to WiFi...");
//   display.display();
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   display.clearDisplay();
//   display.println("WiFi connected!");
//   display.display();

//   // NTP 시간 동기화
//   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
// }

// void loop() {
//   struct tm timeinfo;
//   if (!getLocalTime(&timeinfo)) {
//     Serial.println("Failed to obtain time");
//     return;
//   }

//   // 시간 출력
//   char timeStr[64];
//   strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
//   Serial.println(timeStr);

//   // OLED에 출력
//   display.clearDisplay();
//   display.setCursor(0, 0);
//   display.setTextSize(1);
//   display.println("Current Time:");
//   display.setTextSize(2);
//   display.setCursor(0, 20);
//   display.println(timeStr);
//   display.display();

//   delay(1000);
// }
