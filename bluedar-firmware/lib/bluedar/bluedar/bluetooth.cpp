#include "bluetooth.h"

#include <ArduinoJson.h>
#include <NimBLEDevice.h>
#include "bluedar-config.h"
#include "comms.h"

namespace bluedar::bt {

class BleCallbackClass : public NimBLEScanCallbacks {
    void onResult(const NimBLEAdvertisedDevice* device) override {
        std::string macAddr = device->getAddress().toString();
        discoveredDevices[macAddr] = {device, 10};
    }
};

// void btcCallback(BTAdvertisedDevice* advDevice) {
//     std::string macAddr = advDevice->getAddress().toString().c_str();
//     discoveredDevices[macAddr] = {*advDevice, 10};
// }

void setupBle() {
    uint8_t myAddress[6] = {0x00, 0x00, 0x00, 0x00, 0x00, BLUEDAR_PROBE_ID};
    BLEDevice::init("esp32 ble scanner");
    BLEDevice::setOwnAddrType(0);
    BLEDevice::setOwnAddr(myAddress);
    bleScan = BLEDevice::getScan();
    BLEAdvertisedDeviceCallbacks* callback = new BleCallbackClass();
    // bleScan->setAdvertisedDeviceCallbacks(callback);
    bleScan->setScanCallbacks(callback);
    bleScan->setActiveScan(true);
}

void setupBtc() {
    // btcSerial.begin("esp32 bt scanner");
}

void setupAll() {
    setupBle();
    setupBtc();
}

void printDiscoveredDevices() {
    int btCount = 0, bleCount = 0;
    debug("\n\n\n\n\n\n")

        for (const auto& [macAddr, dev] : discoveredDevices) {
        std::string info = dev.print();
        debug(info.c_str());

        if (dev.deviceType == BtDeviceType::BtClassic) {
            btCount++;
        } else {
            bleCount++;
        }
    }

    std::string total =
        "TOTAL DEVICE COUNT: " + std::to_string(discoveredDevices.size()) +
        " | btc: " + std::to_string(btCount) +
        " | ble: " + std::to_string(bleCount) + "\n";

    debug(total.c_str());
}

JsonDocument payload() {
    JsonDocument doc;
    doc["probe_id"] = (int)BLUEDAR_PROBE_ID;
    doc["time"] = (uint64_t)bluedar::comms::timeClient.getEpochTime();
    Serial.println(doc["time"].as<uint64_t>());
    JsonArray devices = doc["discovered_devices"].to<JsonArray>();
    for (const auto& [macAddr, dev] : discoveredDevices) {
        if (dev.ttl >= dev.initial_ttl - 1) {
            JsonDocument devObj;
            devObj["is_ble"] = true;
            devObj["address"] = macAddr;
            devObj["address_type_id"] = (uint8_t)dev.btAddressTypeId;
            devObj["name"] = dev.name;
            devObj["rssi"] = dev.rssi;
            devObj["appearance_id"] = dev.appearanceId;
            devices.add(devObj);
        }
    }
    return doc;
}

void scan(int seconds) {
    // btcSerial.discoverAsync(bluedar::bt::btcCallback, seconds * 1000);
    bleScan->clearResults();
    bleScan->setInterval(seconds * 1000);
    bleScan->setWindow(seconds * 1000);
    bleScan->start(seconds * 1000);  // `seconds` seconds of nonblocking scan
    delay(seconds * 1000);
    // btcSerial.discoverAsyncStop();
}

void decreaseTtls() {
    for (auto it = discoveredDevices.begin(); it != discoveredDevices.end();) {
        auto& item = it->second;
        item.ttl -= 1;
        if (item.ttl <= 0) {
            it = discoveredDevices.erase(it);
        }
        ++it;
    }
}

}  // namespace bluedar::bt