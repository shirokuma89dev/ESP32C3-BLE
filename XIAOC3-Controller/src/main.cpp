#include <Arduino.h>
#include <BLEDevice.h>

static BLEUUID serviceUUID("fe77e1f2-1e06-11ee-be56-0242ac120002");
static BLEUUID charUUID("060254ca-1e07-11ee-be56-0242ac120002");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
BLECharacteristic* pCharacteristic = NULL;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                           uint8_t* pData, size_t length, bool isNotify) {
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
    }

    void onDisconnect(BLEClient* pclient) {
        connected = false;
    }
};

bool connectToServer() {
    BLEClient* pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());
    pClient->connect(myDevice);
    pClient->setMTU(517);
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        pClient->disconnect();
        return false;
    }
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
        pClient->disconnect();
        return false;
    }

    if (pRemoteCharacteristic->canRead()) {
        std::string value = pRemoteCharacteristic->readValue();
    }

    if (pRemoteCharacteristic->canNotify())
        pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveServiceUUID() &&
            advertisedDevice.isAdvertisingService(serviceUUID)) {
            BLEDevice::getScan()->stop();
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            doConnect = true;
            doScan = true;
        }
    }
};

void setup() {
    Serial.begin(115200);
    BLEDevice::init("STM7_DEVICE2");

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

void loop() {
    if (doConnect == true) {
        connectToServer();
        doConnect = false;
    }

    if (connected) {
        // if (Serial.available() != 0) {
        //     char sendData = Serial.read();
        //     pRemoteCharacteristic->writeValue((uint8_t*)&sendData, 1);
        //     pRemoteCharacteristic->registerForNotify(notifyCallback);
        // }
        if (Serial.available() != 0) {
            int dataSize = 0;
            uint8_t sendDataArr[140] = {0};
            while (Serial.available() != 0) {
                uint8_t data = Serial.read();
                sendDataArr[dataSize] = data;
                dataSize++;
            }

            pRemoteCharacteristic->writeValue((uint8_t*)&sendDataArr, dataSize);
            pRemoteCharacteristic->registerForNotify(notifyCallback);
        }
    } else if (doScan) {
        BLEDevice::getScan()->start(0);
    }
}
