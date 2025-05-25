#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "8bbdd083-12ff-4bbc-bf26-7c540d73d2e8"
#define RGB_CHARACTERISTIC_UUID "3a43cb97-f030-492d-84b3-654a9b2db451"
#define NOTYFY_CHARACTERISTIC_UUID "6a1ca94c-0fca-4f7b-b768-8b615d42169e"

#define RED_PIN 4
#define GREEN_PIN 15
#define BLUE_PIN 2

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pRGBCharacteristic = NULL;
BLECharacteristic *pCharacteristic = NULL;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue().c_str();

    if(value.length() > 0) {
      int delimiterPos1 = value.indexOf(',');
      int delimiterPos2 = value.lastIndexOf(',');

      if(delimiterPos1 != -1 && delimiterPos2 != -1 && delimiterPos2 < value.length() -1) {
        redValue = value.substring(0, delimiterPos1).toInt();
        greenValue = value.substring(delimiterPos1 + 1, delimiterPos2).toInt();
        blueValue = value.substring(delimiterPos2+1).toInt();

        analogWrite(RED_PIN, 255-redValue);
        analogWrite(GREEN_PIN, 255-greenValue);
        analogWrite(BLUE_PIN, 255-blueValue);
      }
    }
  }
};

void setup() {
  BLEDevice::init("RGB LED Control");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);

  pRGBCharacteristic = pService->createCharacteristic(RGB_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  pRGBCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic = pService->createCharacteristic(NOTYFY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  analogWrite(RED_PIN, 255-redValue);
  analogWrite(GREEN_PIN, 255-greenValue);
  analogWrite(BLUE_PIN, 255-blueValue);
}

void loop() {
  char hexValue[8];
  sprintf(hexValue, "#%02X%02X%02X", redValue, greenValue, blueValue);

  pCharacteristic->setValue(hexValue);
  pCharacteristic->notify();

  delay(100);
}