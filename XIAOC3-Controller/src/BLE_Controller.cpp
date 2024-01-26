// #include "BLE_Controller.h"

// ClientCallback::ClientCallback(bool& isConnected) : isConnected(isConnected) {
// }

// void ClientCallback::onConnect(BLEClient* client) {
// }

// void ClientCallback::onDisconnect(BLEClient* client) {
//     isConnected = false;
// }

// AdvertisedDeviceCallback::AdvertisedDeviceCallback(bool& shouldConnect, bool& shouldScan,
//                              BLEUUID& serviceUUID,
//                              BLEAdvertisedDevice*& targetDevice)
//         : shouldConnect(shouldConnect),
//           shouldScan(shouldScan),
//           serviceUUID(serviceUUID),
//           targetDevice(targetDevice) {
// }

// void AdvertisedDeviceCallback::onResult(BLEAdvertisedDevice advertisedDevice) {
//     if (advertisedDevice.haveServiceUUID() &&
//         advertisedDevice.isAdvertisingService(serviceUUID)) {
//         BLEDevice::getScan()->stop();
//         targetDevice = new BLEAdvertisedDevice(advertisedDevice);
//         shouldConnect = true;
//         shouldScan = true;
//     }
// }

// BLE_CONTROLLER::BLE_CONTROLLER(const char* deviceName) {
//     this->deviceName = deviceName;
// }

// void BLE_CONTROLLER::init(void) {
//     BLEDevice::init(deviceName);
//     BLEScan* scan = BLEDevice::getScan();
//     scan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallback(
//         shouldConnect, shouldScan, serviceUUID, targetDevice));
//     scan->setInterval(1349);
//     scan->setWindow(449);
//     scan->setActiveScan(true);
//     scan->start(5, false);
// }

// bool BLE_CONTROLLER::checkConnection(void) {
//     if (shouldConnect == true) {
//         connectToServer();
//         shouldConnect = false;
//     }

//     if (isConnected) {
//         return true;
//     } else {
//         if (shouldScan) {
//             BLEDevice::getScan()->start(0);
//         }
//         return false;
//     }
// }

// void BLE_CONTROLLER::write(uint8_t* sendDataArr, size_t dataSize) {
//     remoteCharacteristic->writeValue(sendDataArr, dataSize);
//     remoteCharacteristic->registerForNotify(onNotificationReceived);
// }

// void BLE_CONTROLLER::onNotificationReceived(
//     BLERemoteCharacteristic* remoteCharacteristic, uint8_t* data, size_t length,
//     bool isNotify) {
//     Serial.println((char*)data);
// }

// bool BLE_CONTROLLER::connectToServer() {
//     BLEClient* client = BLEDevice::createClient();

//     client->setClientCallbacks(new ClientCallback(isConnected));
//     client->connect(targetDevice);
//     client->setMTU(517);

//     BLERemoteService* remoteService = client->getService(serviceUUID);
//     if (remoteService == nullptr) {
//         client->disconnect();
//         return false;
//     }

//     remoteCharacteristic = remoteService->getCharacteristic(charUUID);
//     if (remoteCharacteristic == nullptr) {
//         client->disconnect();
//         return false;
//     }

//     if (remoteCharacteristic->canRead()) {
//         std::string value = remoteCharacteristic->readValue();
//     }

//     if (remoteCharacteristic->canNotify())
//         remoteCharacteristic->registerForNotify(onNotificationReceived);

//     isConnected = true;
//     return true;
// }