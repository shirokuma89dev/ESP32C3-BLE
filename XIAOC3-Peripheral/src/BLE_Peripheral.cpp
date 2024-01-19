#include "BLE_Peripheral.h"

BLE_PERIPHERAL::BLE_PERIPHERAL() : deviceConnected(false), oldDeviceConnected(false), pServer(NULL), pCharacteristic(NULL), value(0) {}

void BLE_PERIPHERAL::init() {
    // Create the BLE Device
    BLEDevice::init("XIAOC3 PeripheralA");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));

    // Create the BLE Service
    BLEService* pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                             BLECharacteristic::PROPERTY_WRITE |
                             BLECharacteristic::PROPERTY_NOTIFY |
                             BLECharacteristic::PROPERTY_INDICATE);

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new MyCallbacks(this));

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

BLE_PERIPHERAL::MyServerCallbacks::MyServerCallbacks(BLE_PERIPHERAL* peripheral) : peripheral(peripheral) {}

void BLE_PERIPHERAL::MyServerCallbacks::onConnect(BLEServer* pServer) {
    peripheral->deviceConnected = true;
}

void BLE_PERIPHERAL::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    peripheral->deviceConnected = false;
}

BLE_PERIPHERAL::MyCallbacks::MyCallbacks(BLE_PERIPHERAL* peripheral) : peripheral(peripheral) {}

void BLE_PERIPHERAL::MyCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++)
            Serial.print(rxValue[i]);
    }
    Serial.println();
}