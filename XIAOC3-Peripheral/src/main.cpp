#include <Arduino.h>

#include "./BLE_Peripheral.h"

BLE_PERIPHERAL BLE_Peripheral("XIAOC3 PeripheralA");

void setup() {
    Serial.begin(115200);
    BLE_Peripheral.init();
}

void loop() {
    if (BLE_Peripheral.checkConnection()) {
        if (Serial.available() != 0) {
            int dataSize = 0;
            char sendDataArr[140] = {0};
            while (Serial.available() != 0) {
                sendDataArr[dataSize] = Serial.read();
                dataSize++;
            }

            BLE_Peripheral.write(sendDataArr, dataSize);
        }

        while (BLE_Peripheral.available() != 0) {
            char data = BLE_Peripheral.read();
            Serial.write(data);
        }
    }
}