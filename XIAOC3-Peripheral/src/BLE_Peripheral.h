#ifndef _BLE_PERIPHERAL_H_
#define _BLE_PERIPHERAL_H_

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Arduino.h>

#define SERVICE_UUID "fe77e1f2-1e06-11ee-be56-0242ac120002"
#define CHARACTERISTIC_UUID "060254ca-1e07-11ee-be56-0242ac120002"

class BLE_PERIPHERAL {
   public:
    // bool deviceConnected = false;
    // bool oldDeviceConnected = false;

    bool deviceConnected = false;
    bool oldDeviceConnected = false;

    BLEServer* pServer = NULL;
    BLECharacteristic* pCharacteristic = NULL;

    uint32_t value = 0;

    class MyServerCallbacks : public BLEServerCallbacks {
        void onConnect(BLEServer* pServer) {
            deviceConnected = true;
        };

        void onDisconnect(BLEServer* pServer) {
            deviceConnected = false;
        }
    };

    class MyCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pCharacteristic) {
            std::string rxValue = pCharacteristic->getValue();

            if (rxValue.length() > 0) {
                for (int i = 0; i < rxValue.length(); i++)
                    Serial.print(rxValue[i]);
            }
            Serial.println();
        }
    };

    void init(void) {
        // Create the BLE Device
        BLEDevice::init("XIAOC3 PeripheralA");

        // Create the BLE Server
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());

        // Create the BLE Service
        BLEService* pService = pServer->createService(SERVICE_UUID);

        pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                                     BLECharacteristic::PROPERTY_WRITE |
                                     BLECharacteristic::PROPERTY_NOTIFY |
                                     BLECharacteristic::PROPERTY_INDICATE);

        pCharacteristic->addDescriptor(new BLE2902());
        pCharacteristic->setCallbacks(new MyCallbacks());

        pService->start();

        BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(false);
        pAdvertising->setMinPreferred(0x0);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06);
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
    };
} BLE_Peripheral;

#endif