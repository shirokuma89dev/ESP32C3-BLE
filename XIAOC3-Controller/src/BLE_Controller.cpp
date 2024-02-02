#include "BLE_Controller.h"

BLE_Controller::BLE_Controller(const char* deviceName,
                               const char* peripheralName) {
    this->_deviceName = deviceName;
    this->_peripheralName = peripheralName;

    uuidGenerate(peripheralName);
}

void BLE_Controller::uuidGenerate(const char* tag) {
    char serviceUUID[38];
    char characteristicUUID[38];
    
    unsigned long uuidTag = 0;
    for (int i = 0; i < strlen(tag); i++) {
        uuidTag += tag[i] * i;
    }
    uuidTag = uuidTag % 2000;

    sprintf(serviceUUID, "%08ld-%04ld-%04ld-%04ld-%012ld", uuidTag,
            uuidTag * 2, uuidTag * 3, uuidTag * 4, uuidTag * 5);

    uuidTag *= 4;
    uuidTag = uuidTag % 2000;

    sprintf(characteristicUUID, "%08ld-%04ld-%04ld-%04ld-%012ld", uuidTag,
            uuidTag * 2, uuidTag * 3, uuidTag * 4, uuidTag * 5);

    this->_serviceUUID = BLEUUID(serviceUUID);
    this->_characteristicUUID = BLEUUID(characteristicUUID);
}

void BLE_Controller::init(void) {
    BLEDevice::init(_deviceName);
    BLEScan* scan = BLEDevice::getScan();

    scan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallback(
        _shouldConnect, _shouldScan, _serviceUUID, _targetDevice));
    scan->setInterval(1349);
    scan->setWindow(449);
    scan->setActiveScan(true);
    scan->start(5, false);
}

bool BLE_Controller::checkConnection(void) {
    if (_shouldConnect == true) {
        connectToServer();
        _shouldConnect = false;
    }

    if (_isConnected) {
        return true;
    } else {
        if (_shouldScan) {
            BLEDevice::getScan()->start(0);
        }
        return false;
    }
}

void BLE_Controller::write(uint8_t* sendDataArr, size_t dataSize) {
    _remoteCharacteristic->writeValue(sendDataArr, dataSize);
    _remoteCharacteristic->registerForNotify(onNotificationReceived);
}

void BLE_Controller::onNotificationReceived(
    BLERemoteCharacteristic* remoteCharacteristic, uint8_t* data, size_t length,
    bool isNotify) {
    Serial.println((char*)data);
}

bool BLE_Controller::connectToServer() {
    BLEClient* client = BLEDevice::createClient();

    client->setClientCallbacks(new ClientCallback(_isConnected));
    client->connect(_targetDevice);
    client->setMTU(517);

    BLERemoteService* remoteService = client->getService(_serviceUUID);
    if (remoteService == nullptr) {
        client->disconnect();
        return false;
    }

    _remoteCharacteristic = remoteService->getCharacteristic(_characteristicUUID);
    if (_remoteCharacteristic == nullptr) {
        client->disconnect();
        return false;
    }

    if (_remoteCharacteristic->canRead()) {
        std::string value = _remoteCharacteristic->readValue();
    }

    if (_remoteCharacteristic->canNotify())
        _remoteCharacteristic->registerForNotify(onNotificationReceived);

    _isConnected = true;
    return true;
}

BLE_Controller::ClientCallback::ClientCallback(bool& isConnected)
    : _isConnected(isConnected) {
}

void BLE_Controller::ClientCallback::onConnect(BLEClient* client) {
}

void BLE_Controller::ClientCallback::onDisconnect(BLEClient* client) {
    _isConnected = false;
}

BLE_Controller::AdvertisedDeviceCallback::AdvertisedDeviceCallback(
    bool& shouldConnect, bool& shouldScan, BLEUUID& serviceUUID,
    BLEAdvertisedDevice*& targetDevice)
    : _shouldConnect(shouldConnect),
      _shouldScan(shouldScan),
      _serviceUUID(serviceUUID),
      _targetDevice(targetDevice) {
}

void BLE_Controller::AdvertisedDeviceCallback::onResult(
    BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() &&
        advertisedDevice.isAdvertisingService(_serviceUUID)) {
        BLEDevice::getScan()->stop();
        _targetDevice = new BLEAdvertisedDevice(advertisedDevice);
        _shouldConnect = true;
        _shouldScan = true;
    }
}