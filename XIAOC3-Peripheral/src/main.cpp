#include <Arduino.h>

#include "./BLE_Peripheral.h"

BLE_PERIPHERAL BLE_Peripheral;

void setup() {
    Serial.begin(115200);
    BLE_Peripheral.init();
}

void loop() {
    // decice connected

    if (BLE_Peripheral.deviceConnected) {
        if (Serial.available() != 0) {
            int dataSize = 0;
            uint8_t sendDataArr[140] = {0};
            while (Serial.available() != 0) {
                uint8_t data = Serial.read();
                sendDataArr[dataSize] = data;
                dataSize++;
            }

            BLE_Peripheral.pCharacteristic->setValue(sendDataArr, dataSize);
            BLE_Peripheral.pCharacteristic->notify();
        }

        delay(2);
    }

    if (!BLE_Peripheral.deviceConnected && BLE_Peripheral.oldDeviceConnected) {
        delay(100);
        BLE_Peripheral.pServer->startAdvertising();
        BLE_Peripheral.oldDeviceConnected = BLE_Peripheral.deviceConnected;

        Serial.println("Waiting a client connection to notify...");
    }
    if (BLE_Peripheral.deviceConnected && !BLE_Peripheral.oldDeviceConnected) {
        Serial.println("Connected");
        BLE_Peripheral.oldDeviceConnected = BLE_Peripheral.deviceConnected;
    }
}