#include "bluetooth.h"

#include <BLEDevice.h>

namespace bluedar::bt {

class BleCallbackClass : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice device) {
        std::string macAddr = device.getAddress().toString();
        discoveredDevices[macAddr] = {device, 30};
    }
};

void btcCallback(BTAdvertisedDevice* advDevice) {
    std::string name = advDevice->getName().c_str();
    std::string macAddr = advDevice->getAddress().toString().c_str();
    uint32_t cod = advDevice->getCOD();
    int8_t rssi = advDevice->getRSSI();
    BtcAdvertisedDevice device = {name, macAddr, cod, rssi};
    discoveredDevices[macAddr] = {device, 30};
}

void setupBle() {
    BLEDevice::init("esp32 ble scanner");
    bleScan = BLEDevice::getScan();
    BLEAdvertisedDeviceCallbacks* callback = new BleCallbackClass();
    bleScan->setAdvertisedDeviceCallbacks(callback);
    bleScan->setActiveScan(true);
}

void setupBtc() {
    btcSerial.begin("esp32 bt scanner");
}

void setupAll() {
    setupBle();
    setupBtc();
}

void printDiscoveredDevices() {
    int btCount = 0, bleCount = 0;
    Serial.println("\n\n\n\n\n\n");
    for (auto it = discoveredDevices.begin(); it != discoveredDevices.end();) {
        auto& item = it->second;
        item.ttl -= 1;
        if (item.ttl < 0) {
            it = discoveredDevices.erase(it);
        } else {
            item.print();
            if (item.getDeviceType() == BtDeviceType::BtClassic) {
                btCount++;
            } else {
                bleCount++;
            }
            ++it;
        }
    }
    Serial.printf("TOTAL DEVICE COUNT: %d | btc: %d | ble: %d\n",
                  discoveredDevices.size(), btCount, bleCount);
}

void scan(int seconds) {
    btcSerial.discoverAsync(bluedar::bt::btcCallback, seconds * 1000);
    bleScan->start(seconds);  // `seconds` seconds of blocking scan
    // This delay is needed for async bt classic scan
    btcSerial.discoverAsyncStop();
}

}  // namespace bluedar::bt