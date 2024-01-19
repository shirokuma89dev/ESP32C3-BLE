#include <Arduino.h>

#include "./BLE_Peripheral.h"

void setup() {
    Serial.begin(115200);

}

void loop() {
    //decice connected
    
    // if (MyServerCallbacks::deviceConnected) {
    //     if (Serial.available() != 0) {
    //         int dataSize = 0;
    //         uint8_t sendDataArr[140] = {0};
    //         while (Serial.available() != 0) {
    //             uint8_t data = Serial.read();
    //             sendDataArr[dataSize] = data;
    //             dataSize++;
    //         }

    //         pCharacteristic->setValue(sendDataArr, dataSize);
    //         pCharacteristic->notify();
    //     }

    //     delay(2);
    // }

    // if (!MyServerCallbacks::deviceConnected && MyServerCallbacks::oldDeviceConnected) {
    //     delay(100);
    //     pServer->startAdvertising();
    //     MyServerCallbacks::oldDeviceConnected = MyServerCallbacks::deviceConnected;

    //     Serial.println("Waiting a client connection to notify...");
    // }
    // if (MyServerCallbacks::deviceConnected && !MyServerCallbacks::oldDeviceConnected) {
    //     Serial.println("Connected");
    //     MyServerCallbacks::oldDeviceConnected = MyServerCallbacks::deviceConnected;
    // }
}