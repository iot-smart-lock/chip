#include "ble.h"
#include <Arduino.h>

class serverCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        Serial.println("BLE-Device connected");
        // connection = true;
    };

    void onDisconnect(BLEServer *pServer) {
        Serial.println("BLE-Device disconnected");
    }
};

void ble_setup() {
    BLEDevice::init("TTGO T-Beam");
    BLEServer *pServer = BLEDevice::createServer();
    pServer -> setCallbacks(new serverCallbacks());
    BLEService *pService = pServer -> createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService -> createCharacteristic(CHARACTERISTIC_UUID,
                                                                          BLECharacteristic::PROPERTY_READ   | 
                                                                          BLECharacteristic::PROPERTY_WRITE  | 
                                                                          BLECharacteristic::PROPERTY_NOTIFY | 
                                                                          BLECharacteristic::PROPERTY_INDICATE
                                                                          );
    pCharacteristic -> setValue("Hallo");
    pService -> start();
    BLEAdvertising *pAdvertising = pServer -> getAdvertising();
    pAdvertising -> addServiceUUID(SERVICE_UUID);
    pAdvertising -> setScanResponse(true);
    pAdvertising -> setMinPreferred(0x06);
    pAdvertising -> setMinPreferred(0x12);
    pAdvertising -> start();
}