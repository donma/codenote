#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <M5Stack.h>
#define SERVICE_UUID        "0000fff0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
String Message = "D51c";
BLEService *pService;
BLEAdvertising *pAdvertising;
BLECharacteristic *pCharacteristic;

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Lcd.println("Starting D51c Leader Device...");

  BLEDevice::init("D51-MASTER");
  BLEServer *pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setValue(Message.c_str());
  pService->start();
  pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Lcd.println("Hey Leader, Already Start Broadcasting..");
  
}


void PrintSign(String text)
{
  int size = 128;
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(size);
  M5.Lcd.setCursor(size * 0.775, size * 0.75);
  M5.Lcd.print(text);
  M5.Lcd.setTextSize(1);
}
void PrintLogo(String text)
{
  int size = 20;
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(size);
  M5.Lcd.setCursor((( M5.Lcd.width() / 2) - (text.length() * size)), (M5.Lcd.height() / 2) - (size));
  M5.Lcd.print(text);
  M5.Lcd.setTextSize(1);
}

void loop() {

  if (M5.BtnA.wasPressed()) {
    Message = "<<";
    pCharacteristic->setValue(Message.c_str());
    PrintSign(Message);
  }
  else if (M5.BtnC.wasPressed()) {
    Message = ">>";
    pCharacteristic->setValue(Message.c_str());
    PrintSign(Message);;
  }
  else if (M5.BtnB.wasPressed()) {
    Message = "D51c";
    pCharacteristic->setValue(Message.c_str());
    PrintLogo(Message);
  }
  M5.update();
  delay(500);
}
