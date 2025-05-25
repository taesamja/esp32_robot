#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "images.h"

// OLED 설정
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C // 128x64:0x3D, 128x32:0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 할당 실패"));
        while(true)
            ;
    }

    display.display();
    delay(2000);

    display.clearDisplay();

    display.drawBitmap(
        0,
        0,
        VeryGood, 32, 32, 1
    );

    display.drawBitmap(
        32,
        0,
        Good, 32, 32, 1
    );

    display.drawBitmap(
        64,
        0,
        Bad, 32, 32, 1
    );

    display.drawBitmap(
        96,
        0,
        VeryBad, 32, 32, 1
    );

    display.display();
}

void loop() {

}


