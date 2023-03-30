#include "ble.h"
#include <Arduino.h>

#define OPEN 0x00
#define LOCKED 0x01

bool locked = false;
BLEServer *pServer = NULL;

void handleCommand(char *command) {
    Serial.print("Command = ");
    Serial.println(command);

    if (command[0] == LOCKED) {
        locked = true;
        Serial.println("Add code for BLE-Action aka. make LED on");
    }
    else if (command[0] == OPEN) {
        locked = false;
        Serial.println("Add code for BLE-Action aka. make LED off");
    }
    else {
        Serial.print("Unknown command: ");
        Serial.println(command);
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

    void onRead(BLECharacteristic *pCharacteristic) {
        std::string value;
        if (locked) {
            value[0] = LOCKED;
        }
        else {
            value[0] = OPEN;
        }
        Serial.printf("onRead: %s", value.c_str());
        // pCharacteristic->setValue(value); //TODO: Doesn't work
    }
};

void advertise() {
    if (pServer == NULL) {
       return;
    }
    BLEAdvertising *pAdvertising = pServer -> getAdvertising();
    pAdvertising -> addServiceUUID(SERVICE_UUID);
    pAdvertising -> setScanResponse(true);
    pAdvertising -> setMinPreferred(0x06);
    pAdvertising -> setMinPreferred(0x12);
    pAdvertising -> start();
}

class serverCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        Serial.println("BLE-Device connected");
    };

    void onDisconnect(BLEServer *pServer) {
        Serial.println("BLE-Device disconnected");
        advertise();
    }
};

void ble_setup() {
    BLEDevice::init("TTGO T-Beam");
    pServer = BLEDevice::createServer();
    pServer -> setCallbacks(new serverCallbacks());
    BLEService *pService = pServer -> createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService -> createCharacteristic(CHARACTERISTIC_UUID,
                                                                          BLECharacteristic::PROPERTY_READ   | 
                                                                          BLECharacteristic::PROPERTY_WRITE  | 
                                                                          BLECharacteristic::PROPERTY_NOTIFY | 
                                                                          BLECharacteristic::PROPERTY_INDICATE
                                                                          );
    pCharacteristic -> setValue("TTGO T-Beam");
    pCharacteristic -> setCallbacks(new serverOperationCallbacks());
    pService -> start();
    advertise();
}