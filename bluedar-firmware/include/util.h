#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BluetoothSerial.h>
#include <variant>

#define BT_DEVICE_INITIAL_TTL (30)

String getBleAppearanceString(uint16_t appearanceId);
String getBleAddressTypeString(int addrType);
String getBtcClassOfDeviceString(uint32_t codType);

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

    void print() {
        switch (deviceType) {
            case BtDeviceType::BtClassic: {
                auto btc = std::get<BtcAdvertisedDevice>(device);
                Serial.print("BTC Device Name: ");
                Serial.println(btc.name.c_str());
                Serial.print("Address: ");
                Serial.println(btc.address.c_str());
                Serial.print("Class of Device: ");
                Serial.println(getBtcClassOfDeviceString(btc.cod));
                Serial.print("RSSI: ");
                Serial.println(btc.rssi);
            } break;

            case BtDeviceType::BtLowEnergy: {
                auto ble = std::get<BLEAdvertisedDevice>(device);
                Serial.print("BLE Device Name: ");
                Serial.println(ble.getName().c_str());
                Serial.print("Address: ");
                Serial.println(ble.getAddress().toString().c_str());
                Serial.print("Address Type: ");
                Serial.println(getBleAddressTypeString(ble.getAddressType()));
                Serial.print("Appearance: ");
                Serial.println(getBleAppearanceString(ble.getAppearance()));
                Serial.print("TX Power: ");
                Serial.println(ble.getTXPower());
                Serial.print("RSSI: ");
                Serial.println(ble.getRSSI());
            } break;
        }

        Serial.printf("TTL: %d out of %d\n", ttl, initial_ttl);
        Serial.println("------------------------");
    }
    BtDeviceType getDeviceType() { return deviceType; }
};
