#ifndef _BLE_CONTROLLER_H_
#define _BLE_CONTROLLER_H_

#include <Arduino.h>
#include <BLEDevice.h>

class BLE_Controller {
   public:
    BLE_Controller(const char* deviceName, const char* peripheralName);

    void enableDebugMode(void);

    void init(void);
    bool checkConnection(void);

    void write(char* sendDataArr, size_t dataSize);

    int available(void);
    char read(void);

   private:
    const char* _deviceName;
    const char* _peripheralName;

    bool _shouldConnect = false;
    bool _isConnected = false;
    bool _shouldScan = false;

    bool _isDebugModeEnabled = false;

    BLEUUID _serviceUUID;
    BLEUUID _characteristicUUID;

    static std::string _rxValue;

    BLERemoteCharacteristic* _remoteCharacteristic = nullptr;
    BLECharacteristic* _localCharacteristic = nullptr;
    BLEAdvertisedDevice* _targetDevice = nullptr;

    void _uuidGenerate(const char* tag);
    bool _connectToServer();

    static void _onNotificationReceived(
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