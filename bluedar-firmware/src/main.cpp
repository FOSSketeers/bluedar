#include <Arduino.h>
#include <BLEDevice.h>
#include <BluetoothSerial.h>
#include <map>
#include "util.h"

std::map<std::string, BtDevice> discoveredDevices;
BluetoothSerial btcSerial;
BLEScan* bleScan;

void btcCallback(BTAdvertisedDevice* advDevice) {
    std::string name = advDevice->getName().c_str();
    std::string macAddr = advDevice->getAddress().toString().c_str();
    uint32_t cod = advDevice->getCOD();
    int8_t rssi = advDevice->getRSSI();
    BtcAdvertisedDevice device = {name, macAddr, cod, rssi};
    discoveredDevices[macAddr] = {device, 30};
}

class BleCallbackClass : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice device) {
        std::string macAddr = device.getAddress().toString();
        discoveredDevices[macAddr] = {device, 30};
    }
};

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

void setup() {
    Serial.begin(115200);
    setupBle();  // Ble = Bluetooth Low Energy
    setupBtc();  // Btc = Bluetooth Classic (BR/EDR)
    Serial.println("outta setup");
}

void loop() {
    btcSerial.discoverAsync(btcCallback, 5000);
    bleScan->start(5);  // 5 seconds of blocking scan
    // This delay is needed for async bt classic scan
    btcSerial.discoverAsyncStop();

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
