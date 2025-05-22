#pragma once

#include <Arduino.h>
#include <NimBLEScan.h>
//#include <BluetoothSerial.h>
#include "BtDevice.h"
#include <ArduinoJson.h>
#include <map>

namespace bluedar::bt {
static std::map<std::string, BtDevice> discoveredDevices;
//static BluetoothSerial btcSerial;
static NimBLEScan* bleScan;

void setupBle();
void setupBtc();
void setupAll();

JsonDocument payload();

void printDiscoveredDevices();
void scan(int seconds);
void decreaseTtls();
}  // namespace bluedar::bt