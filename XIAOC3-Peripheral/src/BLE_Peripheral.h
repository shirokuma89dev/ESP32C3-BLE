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
    BLE_PERIPHERAL();
    void init();

    bool deviceConnected;
    bool oldDeviceConnected;

    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;
    uint32_t value;

   private:
    class MyServerCallbacks : public BLEServerCallbacks {
        BLE_PERIPHERAL* peripheral;

       public:
        MyServerCallbacks(BLE_PERIPHERAL* peripheral);
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
    };

    class MyCallbacks : public BLECharacteristicCallbacks {
        BLE_PERIPHERAL* peripheral;

       public:
        MyCallbacks(BLE_PERIPHERAL* peripheral);
        void onWrite(BLECharacteristic* pCharacteristic);
    };
};

#endif