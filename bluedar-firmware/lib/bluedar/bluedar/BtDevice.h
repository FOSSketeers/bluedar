#pragma once

#include <Arduino.h>
#include <string>
#include <variant>
#include <BluetoothSerial.h>
#include <BLEDevice.h>

#define BT_DEVICE_INITIAL_TTL (10)

namespace bluedar::bt {

enum class BtDeviceType { BtClassic, BtLowEnergy };

// I had to create this barebones clone of BTAdvertisedDevice class because
// of some smart(!) inheritance trickery was causing a bunch of linker errors.
// Inheritance is the root of all evil, gotta keep them all pure.  ~iko
class BtcAdvertisedDevice {
   public:
    std::string name;
    std::string address;
    uint32_t cod;
    int8_t rssi;
};

class BtDevice {
   private:
    int8_t initial_ttl;
    BtDeviceType deviceType;

   public:
    std::variant<BtcAdvertisedDevice, BLEAdvertisedDevice> device;
    int8_t ttl;

    BtDevice() : initial_ttl(BT_DEVICE_INITIAL_TTL) {}

    BtDevice(BtcAdvertisedDevice device)
        : initial_ttl(BT_DEVICE_INITIAL_TTL),
          deviceType(BtDeviceType::BtClassic),
          device(device),
          ttl(BT_DEVICE_INITIAL_TTL) {}

    BtDevice(BtcAdvertisedDevice device, int8_t ttl)
        : initial_ttl(ttl),
          deviceType(BtDeviceType::BtClassic),
          device(device),
          ttl(ttl) {}

    BtDevice(BLEAdvertisedDevice device)
        : initial_ttl(BT_DEVICE_INITIAL_TTL),
          deviceType(BtDeviceType::BtLowEnergy),
          device(device),
          ttl(BT_DEVICE_INITIAL_TTL) {}

    BtDevice(BLEAdvertisedDevice device, int8_t ttl)
        : initial_ttl(ttl),
          deviceType(BtDeviceType::BtLowEnergy),
          device(device),
          ttl(ttl) {}

    void print();
    BtDeviceType getDeviceType();
    String getBleAppearanceString();
    String getBleAddressTypeString();
    String getBtcClassOfDeviceString();
};
}