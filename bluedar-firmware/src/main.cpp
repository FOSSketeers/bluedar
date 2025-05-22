#include <Arduino.h>
#include <bluedar/bluedar-config.h>
#include <bluedar/bluetooth.h>
#include <bluedar/comms.h>
#include <bluedar/led.h>

enum class BluedarState {
    POWER_ON,
    CONNECTING_TO_WIFI,
    SETUP_NTP_AND_MQTT,
    SETUP_BLUETOOTH,
    SCANNING
};

volatile BluedarState appState = BluedarState::POWER_ON;

void bdlog(String s) {
    s = "[bluedar] " + s + "\n";

#ifdef SERIAL_DEBUG
    Serial.print(s);
#elif defined(MQTT_DEBUG)
    mqtt.client.publish(BLUEDAR_MQTT_TOPIC_DEBUG, s.c_str());
#endif
}

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(115200);
#endif
    pinMode(LED_BUILTIN, OUTPUT);
}

void wifiDisconnectEventHandler(arduino_event_id_t) {
    appState = BluedarState::CONNECTING_TO_WIFI;
}

void loop() {
    switch (appState) {
        case BluedarState::POWER_ON: {  // state 0
            bdlog("Powered on");
            appState = BluedarState::CONNECTING_TO_WIFI;
            break;
        }

        case BluedarState::CONNECTING_TO_WIFI: {  // state 1
            bluedar::led::blinkFast();

            bdlog("Connecting to WiFi...");
            WiFi.begin(BLUEDAR_WIFI_SSID, BLUEDAR_WIFI_PASS);
            while (!WiFi.isConnected()) {
                bdlog("Could not connect to WiFi, trying again in 1s...");
                delay(1000);
            }
            bdlog("Connected to WiFi!");
            delay(2000);

            WiFi.onEvent(wifiDisconnectEventHandler,
                         ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

            appState = BluedarState::SETUP_NTP_AND_MQTT;
            break;
        }

        case BluedarState::SETUP_NTP_AND_MQTT: {  // state 2
            bluedar::led::blinkSlow();

            bdlog("Updating clock with NTP");
            bluedar::comms::syncClock();

            bdlog("Connecting to MQTT...");
            bluedar::comms::setupMqtt();
            bdlog("MQTT connection successful!");

            appState = BluedarState::SETUP_BLUETOOTH;
            break;
        }

        case BluedarState::SETUP_BLUETOOTH: {  // state 3
            bluedar::led::toggleOff();

            bdlog("Setting up BLE scanner...");
            bluedar::bt::setupAll();
            bdlog("Ready for scanning!");

            appState = BluedarState::SCANNING;
        }

        case BluedarState::SCANNING: {  // state 4
            bluedar::led::toggleOn();
            bluedar::bt::decreaseTtls();

            bdlog("Scanning...");
            bluedar::bt::scan(BLUEDAR_SCAN_PERIOD_SECONDS);
            bluedar::led::toggleOff();
            bluedar::comms::syncClock();
            bdlog("Publishing...");
            auto payload = bluedar::bt::payload();
            bool ret = bluedar::comms::publish(payload);

#ifndef RELEASE
            bluedar::bt::printDiscoveredDevices();
#endif

            if (!ret) {
                bdlog("Error: could not publish the MQTT payload!");
            }
        }
    }
}
