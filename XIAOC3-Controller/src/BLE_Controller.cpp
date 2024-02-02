#include "BLE_Controller.h"

std::string BLE_Controller::_rxValue = "";

BLE_Controller::BLE_Controller(const char* deviceName,
                               const char* peripheralName) {
    this->_deviceName = deviceName;
    this->_peripheralName = peripheralName;

    _uuidGenerate(peripheralName);
}

void BLE_Controller::enableDebugMode(void) {
    _isDebugModeEnabled = true;
}

void BLE_Controller::init(void) {
    BLEDevice::init(_deviceName);
    if (_isDebugModeEnabled) {
        Serial.println("BLE Device initialized");
        Serial.print("Device name: ");
        Serial.println(_deviceName);
    }

    if (_isDebugModeEnabled) {
        Serial.print("Service UUID: ");
        Serial.println(_serviceUUID.toString().c_str());
        Serial.print("Characteristic UUID: ");
        Serial.println(_characteristicUUID.toString().c_str());
    }

    BLEScan* scan = BLEDevice::getScan();
    if (_isDebugModeEnabled) {
        Serial.println("BLE Scan created");
    }

    scan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallback(
        _shouldConnect, _shouldScan, _serviceUUID, _targetDevice));
    if (_isDebugModeEnabled) {
        Serial.println("Advertised Device Callbacks set");
    }

    scan->setInterval(1349);
    scan->setWindow(449);
    scan->setActiveScan(true);
    if (_isDebugModeEnabled) {
        Serial.println("Scan parameters set");
    }

    scan->start(5, false);
    if (_isDebugModeEnabled) {
        Serial.println("Scan started");
    }
}

bool BLE_Controller::checkConnection(void) {
    if (_shouldConnect == true) {
        _connectToServer();
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

void BLE_Controller::write(char* sendDataArr, size_t dataSize) {
    _remoteCharacteristic->writeValue(sendDataArr, dataSize);
    _remoteCharacteristic->registerForNotify(_onNotificationReceived);
}

int BLE_Controller::available() {
    int length = _rxValue.length();
    return length;
}

char BLE_Controller::read(void) {
    char data = BLE_Controller::_rxValue[0];
    BLE_Controller::_rxValue = BLE_Controller::_rxValue.substr(1);

    return data;
}

void BLE_Controller::_uuidGenerate(const char* tag) {
    char serviceUUID[38];
    char characteristicUUID[38];

    unsigned long uuidTag = 0;
    for (int i = 0; i < strlen(tag); i++) {
        uuidTag += tag[i] * i;
    }
    uuidTag = uuidTag % 2000;

    sprintf(serviceUUID, "%08ld-%04ld-%04ld-%04ld-%012ld", uuidTag, uuidTag * 2,
            uuidTag * 3, uuidTag * 4, uuidTag * 5);

    uuidTag *= 4;
    uuidTag = uuidTag % 2000;

    sprintf(characteristicUUID, "%08ld-%04ld-%04ld-%04ld-%012ld", uuidTag,
            uuidTag * 2, uuidTag * 3, uuidTag * 4, uuidTag * 5);

    this->_serviceUUID = BLEUUID(serviceUUID);
    this->_characteristicUUID = BLEUUID(characteristicUUID);
}

void BLE_Controller::_onNotificationReceived(
    BLERemoteCharacteristic* remoteCharacteristic, uint8_t* data, size_t length,
    bool isNotify) {
    std::string temp = std::string((char*)data, length);
    BLE_Controller::_rxValue = temp;
}

bool BLE_Controller::_connectToServer() {
    BLEClient* client = BLEDevice::createClient();

    client->setClientCallbacks(new ClientCallback(_isConnected));
    client->connect(_targetDevice);
    client->setMTU(517);

    BLERemoteService* remoteService = client->getService(_serviceUUID);
    if (remoteService == nullptr) {
        client->disconnect();
        return false;
    }

    _remoteCharacteristic =
        remoteService->getCharacteristic(_characteristicUUID);
    if (_remoteCharacteristic == nullptr) {
        client->disconnect();
        return false;
    }

    if (_remoteCharacteristic->canRead()) {
        std::string value = _remoteCharacteristic->readValue();
    }

    if (_remoteCharacteristic->canNotify())
        _remoteCharacteristic->registerForNotify(_onNotificationReceived);

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