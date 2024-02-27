#include <Arduino.h>

#include "./BLE_Controller.h"

BLE_Controller ble("BLE_Kit Controller", "BLE_Kit-A");

#include <Adafruit_NeoPixel.h>
#define PIN 20
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);

    delay(2000);

    ble.enableDebugMode();
    ble.init();

    strip.begin();
    strip.setBrightness(50);
    strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.show();
}

void loop() {

    // if (ble.checkConnection()) {
    //     if (Serial.available() != 0) {
    //         int dataSize = 0;
    //         char sendDataArr[140] = {0};
    //         while (Serial.available() != 0) {
    //             sendDataArr[dataSize] = Serial.read();
    //             dataSize++;
    //         }

    //         ble.write(sendDataArr, dataSize);
    //     }

    //     while (ble.available() != 0) {
    //         int length = ble.available();
    //         char dataArr[length] = {0};
    //         for (int i = 0; i < length; i++) {
    //             dataArr[i] = ble.read();
    //         }
    //         Serial.write(dataArr, length);
    //         Serial.write("\n");
    //     }
    // }
}