#include <Arduino.h>

#include "./BLE_Peripheral.h"

BLE_Peripheral ble("BLE_Kit-A");

void setup() {
    Serial.begin(115200);

    delay(2000);

    ble.enableDebugMode();
    ble.init();
}

void loop() {
    if (ble.checkConnection()) {
        if (Serial.available() != 0) {
            int dataSize = 0;
            char sendDataArr[140] = {0};
            while (Serial.available() != 0) {
                sendDataArr[dataSize] = Serial.read();
                dataSize++;
            }

            ble.write(sendDataArr, dataSize);
        }

        while (ble.available() != 0) {
            int length = ble.available();
            char dataArr[length] = {0};
            for (int i = 0; i < length; i++) {
                dataArr[i] = ble.read();
            }
            Serial.write(dataArr, length);
            Serial.write("\n");
        }
    }
}