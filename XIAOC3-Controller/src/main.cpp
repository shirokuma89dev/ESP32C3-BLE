#include <Arduino.h>
#include "./BLE_Controller.h"

// UUIDs for BLE service and characteristic
static BLEUUID serviceUUID("fe77e1f2-1e06-11ee-be56-0242ac120002");
static BLEUUID charUUID("060254ca-1e07-11ee-be56-0242ac120002");

// Flags for BLE connection status
static boolean shouldConnect = false;
static boolean isConnected = false;
static boolean shouldScan = false;

// Characteristics for BLE communication
static BLERemoteCharacteristic* remoteCharacteristic;
BLECharacteristic* localCharacteristic = NULL;

// Device to connect to
static BLEAdvertisedDevice* targetDevice;

// Callback for receiving notifications from BLE device
static void onNotificationReceived(BLERemoteCharacteristic* remoteCharacteristic,
                           uint8_t* data, size_t length, bool isNotify) {
    Serial.println((char*)data);
}

// Callbacks for BLE client
class ClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* client) {
    }

    void onDisconnect(BLEClient* client) {
        isConnected = false;
    }
};

// Function to connect to the BLE server
bool connectToServer() {
    BLEClient* client = BLEDevice::createClient();

    client->setClientCallbacks(new ClientCallback());
    client->connect(targetDevice);
    client->setMTU(517);

    BLERemoteService* remoteService = client->getService(serviceUUID);
    if (remoteService == nullptr) {
        client->disconnect();
        return false;
    }

    remoteCharacteristic = remoteService->getCharacteristic(charUUID);
    if (remoteCharacteristic == nullptr) {
        client->disconnect();
        return false;
    }

    if (remoteCharacteristic->canRead()) {
        std::string value = remoteCharacteristic->readValue();
    }

    if (remoteCharacteristic->canNotify())
        remoteCharacteristic->registerForNotify(onNotificationReceived);

    isConnected = true;
    return true;
}

// Callback for receiving advertised devices
class AdvertisedDeviceCallback : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveServiceUUID() &&
            advertisedDevice.isAdvertisingService(serviceUUID)) {
            BLEDevice::getScan()->stop();
            targetDevice = new BLEAdvertisedDevice(advertisedDevice);
            shouldConnect = true;
            shouldScan = true;
        }
    }
};

void setup() {
    Serial.begin(115200);

    BLEDevice::init("STM7_DEVICE2");

    BLEScan* scan = BLEDevice::getScan();
    scan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallback());
    scan->setInterval(1349);
    scan->setWindow(449);
    scan->setActiveScan(true);
    scan->start(5, false);
}

void loop() {
    if (shouldConnect == true) {
        connectToServer();
        shouldConnect = false;
    }

    if (isConnected) {
        if (Serial.available() != 0) {
            int dataSize = 0;
            uint8_t sendDataArr[140] = {0};
            while (Serial.available() != 0) {
                uint8_t data = Serial.read();
                sendDataArr[dataSize] = data;
                dataSize++;
            }

            remoteCharacteristic->writeValue((uint8_t*)&sendDataArr, dataSize);
            remoteCharacteristic->registerForNotify(onNotificationReceived);
        }
    } else if (shouldScan) {
        BLEDevice::getScan()->start(0);
    }
}