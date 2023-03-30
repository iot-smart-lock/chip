#include "ble.h"
#include <Arduino.h>

class serverCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        Serial.println("BLE-Device connected");
    };

    void onDisconnect(BLEServer *pServer) {
        Serial.println("BLE-Device disconnected");
    }
};


void handleCommand(char *command) {
    Serial.print("Command = ");
    Serial.println(command);

    if (command[0] == 'L') {
        Serial.println("Add code for BLE-Action aka. make LED on");
    }
    else {
        Serial.print("Unknown command: ");
        Serial.println(command[0]);
    }
}

class serverOperationCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic -> getValue();
        if (rxValue.length() > 0) {
            char command[21];
            for (int i = 0; i < rxValue.length(); i++) {
                command[i] = rxValue[i];
            }
            command[rxValue.length()] = '\0';
            handleCommand(command);
        }
    }

    
};

void ble_setup() {
    BLEDevice::init("SmartLock");
    BLEServer *pServer = BLEDevice::createServer();
    pServer -> setCallbacks(new serverCallbacks());
    BLEService *pService = pServer -> createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService -> createCharacteristic(CHARACTERISTIC_UUID,
                                                                          BLECharacteristic::PROPERTY_READ   | 
                                                                          BLECharacteristic::PROPERTY_WRITE  | 
                                                                          BLECharacteristic::PROPERTY_NOTIFY | 
                                                                          BLECharacteristic::PROPERTY_INDICATE
                                                                          );
    pCharacteristic -> setValue("SmartLock");
    pCharacteristic -> setCallbacks(new serverOperationCallbacks());
    pService -> start();
    BLEAdvertising *pAdvertising = pServer -> getAdvertising();
    pAdvertising -> addServiceUUID(SERVICE_UUID);
    pAdvertising -> setScanResponse(true);
    pAdvertising -> setMinPreferred(0x06);
    pAdvertising -> setMinPreferred(0x12);
    pAdvertising -> start();
}