#include <M5Stack.h>
#include <BLEDevice.h>

#define CF_Y32  &Yellowtail_32
#define CF_OL32 &Orbitron_Light_32
int bitghtness = 10;
bool IsPicSupport = true;
bool IsConnectedD51 = false;
bool brightness = 10;
// BLE Block
// The remote service we wish to connect to.
static BLEUUID serviceUUID("0000fff0-0000-1000-8000-00805f9b34fb");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");


static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

int lines = 0;
bool IsOpenBle = false;
//--------------------------------------------------------------------End BLE BLOCK

//Class BLE

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  M5.Lcd.print("NotifyChar:");
  M5.Lcd.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
}

bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());
  M5.Lcd.print("Forming a connection to ");
  M5.Lcd.println(pAddress.toString().c_str());

  BLEClient*  pClient  = BLEDevice::createClient();
  M5.Lcd.println(">>Created client:");
  pClient->connect(pAddress);
  M5.Lcd.println(">>Connected to server");
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    M5.Lcd.print("Failed to find our service UUID: ");
    M5.Lcd.println(serviceUUID.toString().c_str());
    return false;
  }
  M5.Lcd.println(">>Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    M5.Lcd.print("Failed to find our characteristic UUID: ");
    M5.Lcd.println(charUUID.toString().c_str());
    lines++;
    return false;
  }
  Serial.println(">>Found D51 Device");

  // Read the value of the characteristic.
  std::string value = pRemoteCharacteristic->readValue();
  M5.Lcd.print("Server Message:");
  M5.Lcd.println(value.c_str());
  lines++;
  if (value.c_str() == ">>") {
    M5.Lcd.println("RIGHT");
    lines++;
  }
  if (value.c_str() == "<<") {
    M5.Lcd.println("LEFT");
    lines++;
  }
  pRemoteCharacteristic->registerForNotify(notifyCallback);
}

/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    String v1 = "[";
    String v2 = "]";
    String v3 = ">";
    void onResult(BLEAdvertisedDevice advertisedDevice) {

      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());
      Serial.println(advertisedDevice.getRSSI());

      if (20 < lines ) {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.drawJpgFile(SD, "/p1.jpg");
        M5.Lcd.setCursor(0, 0);
        lines = 0;
      }

      if (advertisedDevice.getName().length() > 0)
      {
        if (!IsConnectedD51) {
          lines++;
          M5.Lcd.print("\\");
          M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
          M5.Lcd.print(advertisedDevice.getName().c_str());

          M5.Lcd.print(":");
          M5.Lcd.println(advertisedDevice.getRSSI());
        }
      }
      else {
        //M5.Lcd.print( v1 + advertisedDevice.getAddress().toString().c_str() + v2 );
      }

      if (advertisedDevice.haveServiceUUID()) {
        if (!IsConnectedD51) {
          lines++;
          M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
          M5.Lcd.print( v3 + advertisedDevice.getServiceUUID().toString().c_str());
          M5.Lcd.print(":");
          M5.Lcd.println(advertisedDevice.getRSSI());
        }
      }


      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)
          && ( String(advertisedDevice.getName().c_str()).indexOf("D51") >= 0)) {

        IsConnectedD51 = true;
        lines++;
        M5.Lcd.print("\\");
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
        M5.Lcd.print("Gotcha!!address=>");
        M5.Lcd.print(advertisedDevice.getName().c_str());
        advertisedDevice.getScan()->stop();
        M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

        pServerAddress = new BLEAddress(advertisedDevice.getAddress());
        doConnect = true;

      } // Found our server
    } // onResult
}; // MyAdvertisedDeviceCallbacks

//---------------------------------------------------------------------End Class BLE

void setup(void) {
  //Basic Init
  M5.begin();
  M5.Lcd.setBrightness(bitghtness);
  //M5.Lcd.drawJpgFile(SD, "/p2.jpg");
  if (IsPicSupport)
  {
    M5.Lcd.drawJpgFile(SD, "/p2.jpg");
  } else {
    PrintLogo("D51c");
  }
  M5.setWakeupButton(BUTTON_B_PIN);
  //BLE Init

}

void InitBLE() {
  BLEDevice::init("D51C");
  M5.Lcd.println("BLE Starting...");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  IsOpenBle = true;
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawJpgFile(SD, "/p1.jpg");
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

void LoadRight() {
  if (IsPicSupport)
  {
    M5.Lcd.drawJpgFile(SD, "/right.jpg");
  } else {
    PrintSign(">>");
  }
}

void LoadLeft() {
  if (IsPicSupport)
  {
    M5.Lcd.drawJpgFile(SD, "/left.jpg");
  } else {
    PrintSign("<<");
  }

}
void LoadChloe() {
  //M5.Lcd.drawJpgFile(SD, "/ccc.jpg");
  M5.Lcd.fillScreen(TFT_BLACK);
  if (IsPicSupport)
  {
    M5.Lcd.drawJpgFile(SD, "/ccc.jpg");
  }
  else {
    PrintLogo("BYE");
  }
}

void DisplayLoop() {

  if (M5.BtnA.pressedFor(3000)) {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.powerOFF();
  }
  if (M5.BtnA.pressedFor(2000)) {
    LoadChloe();
  }
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillScreen(TFT_BLACK);
    if (IsPicSupport)
    {
      M5.Lcd.drawJpgFile(SD, "/p2.jpg");
    } else {
      PrintLogo("D51c");
    }
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("ButtonA Long press for close.");
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.println("ButtonB press for Receiving D51c signal.");
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.println("ButtonC press for changing brightness.");
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.println("Power by no2studio & D51c");
  }
  if (M5.BtnB.wasPressed()) {
    InitBLE();
  }

  if (M5.BtnC.wasPressed()) {
    brightness+=50;
    if(brightness>=250){
      brightness=50;
    }
    M5.Lcd.setBrightness(brightness);
    M5.update();
  }
  M5.update();
}

void BLELOOP() {
  if (!IsOpenBle) {
    return;
  }
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      M5.Lcd.println("We are now connected to the BLE Server....");
      connected = true;
    } else {
      M5.Lcd.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  if (connected) {
    if (IsConnectedD51)
    {
      std::string value = pRemoteCharacteristic->readValue();
      if (value == ">>") {
        LoadRight();
      }
      else if (value == "<<") {
        LoadLeft();
      }
      else if (value == "D51c") {
        if (IsPicSupport)
        {
          M5.Lcd.drawJpgFile(SD, "/p2.jpg");
        } else {
          PrintLogo("D51c");
        }
      }
      else {
        M5.Lcd.println(value.c_str());
      }
    }

  }

  BLEScan* pBLEScan = BLEDevice::getScan();
  //  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  //  pBLEScan->setActiveScan(true);
  pBLEScan->start(1);

  delay(500); // Delay a second between loops.

}

void loop() {
  DisplayLoop();
  BLELOOP();
}

