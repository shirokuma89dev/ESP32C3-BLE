#include <Arduino.h>

#include "./BLE_Peripheral.h"

BLE_PERIPHERAL BLE_Peripheral("XIAOC3 PeripheralB");

// const int hash = "XIAOC3 PeripheralA";

void setup() {
    delay(1000);

    Serial.begin(115200);
    BLE_Peripheral.enableDebugMode();
    BLE_Peripheral.init();
}

void loop() {
    if (BLE_Peripheral.checkConnection()) {
        // if (Serial.available() != 0) {
        //     int dataSize = 0;
        //     char sendDataArr[140] = {0};
        //     while (Serial.available() != 0) {
        //         sendDataArr[dataSize] = Serial.read();
        //         dataSize++;
        //     }

        //     BLE_Peripheral.write(sendDataArr, dataSize);
        // }
        static unsigned long flagTimer = millis();
        if (millis() - flagTimer > 1000) {
            char sendDataArr[] = "オーレオレマツケンサンバ〜！";
            int dataSize = sizeof(sendDataArr) / sizeof(sendDataArr[0]);

            BLE_Peripheral.write(sendDataArr, dataSize);

            flagTimer = millis();
        }

        while (BLE_Peripheral.available() != 0) {
            char data = BLE_Peripheral.read();
            Serial.write(data);
        }
    }
}