#ifndef BLE_Controller_H
#define BLE_Controller_H

#include <Arduino.h>
#include <BLEDevice.h>

class BLE_Controller {
   public:
    BLE_Controller(const char* deviceName, const char* peripheralName);

    void init(void);
    bool checkConnection(void);

    void write(uint8_t* sendDataArr, size_t dataSize);

   private:
    const char* _deviceName;
    const char* _peripheralName;

    bool _shouldConnect = false;
    bool _isConnected = false;
    bool _shouldScan = false;

    BLEUUID _serviceUUID;
    BLEUUID _characteristicUUID;

    BLERemoteCharacteristic* _remoteCharacteristic = nullptr;
    BLECharacteristic* _localCharacteristic = nullptr;
    BLEAdvertisedDevice* _targetDevice = nullptr;

    void uuidGenerate(const char* tag);
    bool connectToServer();

    static void onNotificationReceived(
        BLERemoteCharacteristic* remoteCharacteristic, uint8_t* data,
        size_t length, bool isNotify);

    class ClientCallback : public BLEClientCallbacks {
       public:
        ClientCallback(bool& isConnected);

        void onConnect(BLEClient* client);
        void onDisconnect(BLEClient* client);

       private:
        bool& _isConnected;
    };

    class AdvertisedDeviceCallback : public BLEAdvertisedDeviceCallbacks {
       public:
        AdvertisedDeviceCallback(bool& shouldConnect, bool& shouldScan,
                                 BLEUUID& serviceUUID,
                                 BLEAdvertisedDevice*& targetDevice);

        void onResult(BLEAdvertisedDevice advertisedDevice);

       private:
        bool& _shouldConnect;
        bool& _shouldScan;

        BLEUUID& _serviceUUID;
        BLEAdvertisedDevice*& _targetDevice;
    };
};

#endif