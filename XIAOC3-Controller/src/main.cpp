#include <Arduino.h>
#include "./BLE_Controller.h"

BLE_CONTROLLER BLE_controller("XIAOC3 Controller", "XIAOC3 PeripheralB");

void setup() {
    Serial.begin(115200);

    BLE_controller.init();
}

void loop() {
    if (BLE_controller.checkConnection()) {
        if (Serial.available() != 0) {
            int dataSize = 0;
            uint8_t sendDataArr[140] = {0};
            while (Serial.available() != 0) {
                uint8_t data = Serial.read();
                sendDataArr[dataSize] = data;
                dataSize++;
            }

            BLE_controller.write(sendDataArr, dataSize);
        }
    }
}