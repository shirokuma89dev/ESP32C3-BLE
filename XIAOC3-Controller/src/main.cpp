#include <Arduino.h>
#include "./BLE_Controller.h"

BLE_Controller ble("XIAOC3 Controller", "BLE_Kit-A");

void setup() {
    Serial.begin(115200);

    ble.init();
}

void loop() {
    if (ble.checkConnection()) {
        if (Serial.available() != 0) {
            int dataSize = 0;
            uint8_t sendDataArr[140] = {0};
            while (Serial.available() != 0) {
                uint8_t data = Serial.read();
                sendDataArr[dataSize] = data;
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
        }
    }
}