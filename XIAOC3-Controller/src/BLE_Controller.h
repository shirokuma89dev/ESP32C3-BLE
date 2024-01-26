#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include <Arduino.h>
#include <BLEDevice.h>

class BLE_CONTROLLER {
   private:
    char _serviceUuid[38];
    char _characteristicUuid[38];

   public:
    class ClientCallback : public BLEClientCallbacks {
       public:
        ClientCallback(bool& isConnected) : isConnected(isConnected) {
        }

        void onConnect(BLEClient* client) {
        }

        void onDisconnect(BLEClient* client) {
            isConnected = false;
        }

       private:
        bool& isConnected;
    };

    class AdvertisedDeviceCallback : public BLEAdvertisedDeviceCallbacks {
       public:
        AdvertisedDeviceCallback(bool& shouldConnect, bool& shouldScan,
                                 BLEUUID& serviceUUID,
                                 BLEAdvertisedDevice*& targetDevice)
            : shouldConnect(shouldConnect),
              shouldScan(shouldScan),
              serviceUUID(serviceUUID),
              targetDevice(targetDevice) {
        }

        void onResult(BLEAdvertisedDevice advertisedDevice) {
            if (advertisedDevice.haveServiceUUID() &&
                advertisedDevice.isAdvertisingService(serviceUUID)) {
                BLEDevice::getScan()->stop();
                targetDevice = new BLEAdvertisedDevice(advertisedDevice);
                shouldConnect = true;
                shouldScan = true;
            }
        }

       private:
        bool& shouldConnect;
        bool& shouldScan;
        BLEUUID& serviceUUID;
        BLEAdvertisedDevice*& targetDevice;
    };
    
    BLEUUID serviceUUID;
    BLEUUID charUUID;

    // Flags for BLE connection status
    boolean shouldConnect = false;
    boolean isConnected = false;
    boolean shouldScan = false;

    // Characteristics for BLE communication
    BLERemoteCharacteristic* remoteCharacteristic = nullptr;
    BLECharacteristic* localCharacteristic = nullptr;

    // Device to connect to
    BLEAdvertisedDevice* targetDevice = nullptr;

    const char* deviceName;
    const char* peripheralName;

    BLE_CONTROLLER(const char* deviceName, const char* peripheralName) {
        this->deviceName = deviceName;
        this->peripheralName = peripheralName;
        uuidGenerate(peripheralName);
        serviceUUID = BLEUUID(_serviceUuid);
        charUUID = BLEUUID(_characteristicUuid);
    }

    void uuidGenerate(const char* tag) {
        // tagから適当にUUIDらしきものを生成する
        unsigned long uuidTag = 0;
        for (int i = 0; i < strlen(tag); i++) {
            uuidTag += tag[i] * i;
        }
        uuidTag = uuidTag % 2000;

        sprintf(_serviceUuid, "%08ld-%04ld-%04ld-%04ld-%012ld", uuidTag,
                uuidTag * 2, uuidTag * 3, uuidTag * 4, uuidTag * 5);

        uuidTag *= 4;
        uuidTag = uuidTag % 2000;

        sprintf(_characteristicUuid, "%08ld-%04ld-%04ld-%04ld-%012ld", uuidTag,
                uuidTag * 2, uuidTag * 3, uuidTag * 4, uuidTag * 5);
    }

    void init(void) {
        BLEDevice::init(deviceName);
        BLEScan* scan = BLEDevice::getScan();

        scan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallback(
            shouldConnect, shouldScan, serviceUUID, targetDevice));
        scan->setInterval(1349);
        scan->setWindow(449);
        scan->setActiveScan(true);
        scan->start(5, false);
    }

    bool checkConnection(void) {
        if (shouldConnect == true) {
            connectToServer();
            shouldConnect = false;
        }

        if (isConnected) {
            return true;
        } else {
            if (shouldScan) {
                BLEDevice::getScan()->start(0);
            }
            return false;
        }
    }

    void write(uint8_t* sendDataArr, size_t dataSize) {
        remoteCharacteristic->writeValue(sendDataArr, dataSize);
        remoteCharacteristic->registerForNotify(onNotificationReceived);
    }

    // Callback for receiving notifications from BLE device
    static void onNotificationReceived(
        BLERemoteCharacteristic* remoteCharacteristic, uint8_t* data,
        size_t length, bool isNotify) {
        Serial.println((char*)data);
    }

    // Function to connect to the BLE server
    bool connectToServer() {
        BLEClient* client = BLEDevice::createClient();

        client->setClientCallbacks(new ClientCallback(isConnected));
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
};

#endif