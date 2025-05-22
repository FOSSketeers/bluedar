#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>
// #include <BluetoothSerial.h>
#include <string>

#define BT_DEVICE_INITIAL_TTL (10)

namespace bluedar::bt {

enum class BtDeviceType : uint8_t { BtClassic, BtLowEnergy };
enum class BtAddressType : uint8_t {
    INVALID,
    BT_CLASSIC,
    BLE_PUBLIC,
    BLE_RANDOM_STATIC,
    BLE_RANDOM_PRIVATE_RESOLVABLE,
    BLE_RANDOM_PRIVATE_NONRESOLVABLE
};

class BtDevice {
   public:
    int8_t initial_ttl;
    BtDeviceType deviceType;
    std::string name;
    std::string address;
    BtAddressType btAddressTypeId;
    uint32_t appearanceId;  // uint16_t for BLE, uint32_t for BTC.
    int8_t rssi;

    int8_t ttl;

    BtDevice();

    //    BtDevice(BTAdvertisedDevice& device);

    //    BtDevice(BTAdvertisedDevice& device, int8_t ttl);

    BtDevice(const NimBLEAdvertisedDevice* device);

    BtDevice(const NimBLEAdvertisedDevice* device, int8_t ttl);

    std::string print() const;
    String getBleAppearanceString();
    String getBleAddressTypeString();
    String getBtcClassOfDeviceString();
};
}  // namespace bluedar::bt