#include <WiFi.h>
#include <time.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// OLED 설정
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WiFi 설정
#include "secrets.h"

// NTP 서버 설정
int GMTOffset = 60 * 60 * 9;  // 한국은 UTC+9
int daylightOffset = 0;

const String weekDays[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup() {
  Serial.begin(115200);

  // OLED 초기화
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 초기화 실패");
    while(true)
      ;
  }
  
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);

  // WiFi 연결
  WiFi.begin(ssid, password);
  display.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting...");
  }
  Serial.println("Connected to Wi-Fi!");

  configTime(GMTOffset, daylightOffset, "pool.ntp.org", "time.nist.gov");
}

void loop() {

  // 현재 시간 가져오기
  time_t rawtime = time(nullptr);
  struct tm *timeinfo = localtime(&rawtime);

  Serial.print(timeinfo->tm_year + 1900);
  Serial.print("/");
  Serial.print(timeinfo->tm_mon + 1);
  Serial.print(":");
  Serial.print(timeinfo->tm_mday);
  Serial.print(" ");

  Serial.print("Time:");
  Serial.print(timeinfo->tm_hour);
  Serial.print(":");
  Serial.print(timeinfo->tm_min);
  Serial.print(":");
  Serial.print(timeinfo->tm_sec);

  // OLED에 출력
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  
  if(timeinfo->tm_hour < 10) {
    display.print("0"); // 10보다 작은 경우 앞에 0을 채움
  }
  display.print(timeinfo->tm_hour);
  display.print(":");

  if(timeinfo->tm_min < 10) {
    display.print("0"); // 10보다 작은 경우 앞에 0을 채움
  }
  display.print(timeinfo->tm_min);
  display.print(":");


  display.setTextSize(2);
  display.setCursor(102, 5);
  if(timeinfo->tm_sec < 10) {
    display.print("0"); // 10보다 작은 경우 앞에 0을 채움
  }
  display.print(timeinfo->tm_sec);

  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print(timeinfo->tm_year + 1900);
  display.print("/");
  display.print(timeinfo->tm_mon + 1);
  display.print("/");
  display.print(timeinfo->tm_mday);
  display.print(" ");
  display.print(weekDays[timeinfo->tm_wday]);

  display.display();
  
  delay(1000);
}
