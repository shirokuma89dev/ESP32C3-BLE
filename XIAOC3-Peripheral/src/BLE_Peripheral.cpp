#include "BLE_Peripheral.h"

BLE_PERIPHERAL::BLE_PERIPHERAL(const char* deviceName)
    : _isDeviceConnected(false),
      _wasDeviceConnected(false),
      _pServer(NULL),
      _pCharacteristic(NULL) {
    _deviceName = deviceName;
}

void BLE_PERIPHERAL::init() {
    // Create the BLE Device
    BLEDevice::init(_deviceName);

    // Create the BLE Server
    _pServer = BLEDevice::createServer();
    _pServer->setCallbacks(new MyServerCallbacks(this));

    // Create the BLE Service
    BLEService* pService = _pServer->createService(_serviceUuid);

    _pCharacteristic = pService->createCharacteristic(
        _characteristicUuid, BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_WRITE |
                                 BLECharacteristic::PROPERTY_NOTIFY |
                                 BLECharacteristic::PROPERTY_INDICATE);

    _pCharacteristic->addDescriptor(new BLE2902());
    _pCharacteristic->setCallbacks(new MyCallbacks(this));

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(_serviceUuid);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

bool BLE_PERIPHERAL::checkConnection() {
    if (!_isDeviceConnected && _wasDeviceConnected) {
        // delay(100);
        _pServer->startAdvertising();
        _wasDeviceConnected = _isDeviceConnected;

        Serial.println("Waiting a client connection to notify...");
    }
    if (_isDeviceConnected && !_wasDeviceConnected) {
        Serial.println("Connected");
        _wasDeviceConnected = _isDeviceConnected;
    }

    return _isDeviceConnected;
}

int BLE_PERIPHERAL::available() {
    return _rxValue.length();
}

char BLE_PERIPHERAL::read() {
    char data = _rxValue[0];
    _rxValue = _rxValue.substr(1);
    return data;
}

void BLE_PERIPHERAL::write(char* data, size_t length) {
    _pCharacteristic->setValue((uint8_t*)data, length);
    _pCharacteristic->notify();
}

BLE_PERIPHERAL::MyServerCallbacks::MyServerCallbacks(BLE_PERIPHERAL* peripheral)
    : peripheral(peripheral) {
}

void BLE_PERIPHERAL::MyServerCallbacks::onConnect(BLEServer* pServer) {
    peripheral->_isDeviceConnected = true;
}

void BLE_PERIPHERAL::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    peripheral->_isDeviceConnected = false;
}

BLE_PERIPHERAL::MyCallbacks::MyCallbacks(BLE_PERIPHERAL* peripheral)
    : peripheral(peripheral) {
}

void BLE_PERIPHERAL::MyCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    peripheral->_rxValue = pCharacteristic->getValue();
}