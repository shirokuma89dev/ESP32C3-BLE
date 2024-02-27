#include <Arduino.h>

#include "./BLE_Controller.h"

BLE_Controller ble("BLE_Kit Central", "BLE_Kit-A");

#include "./led.h"
Led leds;

#include "./parking.h"
Parking parking;

void readSerialData(void) {
    if (Serial.available() != 0) {
        char receivedData = Serial.read();
        char id = receivedData - 'A';

        if (id < 0 || id > 3) {
            Serial.println("Invalid Parking ID");
            return;
        }

        if (parking.status[id] != Parking::Available) {
            Serial.print("Parking ");
            Serial.write(id + 'A');
            Serial.print(" is ");

            if (parking.status[id] == Parking::Occupied) {
                Serial.println("Occupied");
            } else {
                Serial.println("Reserved");
            }
            return;
        }

        parking.status[id] = Parking::Reserved;
        parking.isApproaching = true;
        parking.approachingParkingId = id;

        Serial.print("Parking ");
        Serial.write(id + 'A');
        Serial.println(" is Available!");
    }
}

void setup() {
    Serial.begin(115200);
    leds.init();

    ble.enableDebugMode();
    ble.init();

    Serial.println("BLE Central is ready!");
}

void loop() {
    readSerialData();

    if (leds.setBLE_Status(ble.checkConnection())) {
        while (ble.available() != 0) {
            if (ble.read() != 'O') {
                continue;
            }

            parking.status[parking.approachingParkingId] = Parking::Occupied;
            parking.isApproaching = false;

            Serial.print("Parking ");
            Serial.write(parking.approachingParkingId + 'A');
            Serial.println(" is Occupied!");

            while (ble.available() != 0) {
                ble.read();
            }

            break;
        }
    }

    leds.setParkingStatus(parking.status);
    leds.show();
}