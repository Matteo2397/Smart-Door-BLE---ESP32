#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "********-****-****-****-************"
#define CHARACTERISTIC_UUID "********-****-****-****-************"

#define Relay     4   //GPIO connected to the relay

class Callback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.print("Recived: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        if(value=="open") {
          digitalWrite(Relay, HIGH);
          Serial.println("ON");
          delay(200);  // wait for a 200 ms
          digitalWrite(Relay, LOW);
          Serial.println("OFF");
        }
        else {
          Serial.println("Unknown command");          
        }
      }
    }
};

void setup() {
  Serial.begin(115200);

  pinMode(Relay, OUTPUT);

  BLEDevice::init("Smart Door");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new Callback());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
