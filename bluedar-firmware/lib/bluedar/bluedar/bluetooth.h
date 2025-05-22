#pragma once

#include <Arduino.h>
#include <BLEScan.h>
#include <BluetoothSerial.h>
#include "BtDevice.h"

namespace bluedar::bt {
static std::map<std::string, BtDevice> discoveredDevices;
static BluetoothSerial btcSerial;
static BLEScan* bleScan;

void setupBle();
void setupBtc();
void setupAll();

void printDiscoveredDevices();
void scan(int seconds);
}  // namespace bluedar::bt