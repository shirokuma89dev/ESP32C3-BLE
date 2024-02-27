#include <Arduino.h>

#include "./BLE_Peripheral.h"
BLE_Peripheral ble("BLE_Kit-A");

#include "./IO-Kit/IO-Kit.h"
Input reflector[3] = {Input(A0), Input(A2), Input(A1)};
Output servoR = Output(21);
Output servoL = Output(7);

#include "./led.h"
Led led;

void setup() {
    Serial.begin(115200);
    led.init();

    ble.enableDebugMode();
    ble.init();
}

void loop() {
    for (int i = 0; i < 3; i++) {
        Serial.print(reflector[i].analog());
        Serial.print(" ");
    }
    Serial.println();

    if (led.setBLE_Status(ble.checkConnection())) {
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
    led.show();
}