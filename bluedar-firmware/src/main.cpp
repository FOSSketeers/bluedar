#include <Arduino.h>
#include <bluedar/bluetooth.h>
#include <bluedar/led.h>

enum class BluedarState {
    POWER_ON,
    CONNECTING_TO_WIFI,
    SETUP_NTP_AND_MQTT,
    SETUP_BLUETOOTH,
    SCANNING
};

volatile BluedarState appState = BluedarState::POWER_ON;
constexpr int scanPeriodSeconds = 5;

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(115200);
#endif
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    switch (appState) {
        case BluedarState::POWER_ON: {  // state 0
            appState = BluedarState::CONNECTING_TO_WIFI;
            break;
        }

        case BluedarState::CONNECTING_TO_WIFI: {  // state 1
            bluedar::led::blinkFast();
            // TODO connect to wifi (sync)
            // TODO wifi disconnect event handler sets appState to 1 (async)
            appState = BluedarState::SETUP_NTP_AND_MQTT;
            break;
        }

        case BluedarState::SETUP_NTP_AND_MQTT: {  // state 2
            bluedar::led::blinkSlow();
            // TODO set up ntp
            // TODO connect to mqtt
            appState = BluedarState::SETUP_BLUETOOTH;
            break;
        }

        case BluedarState::SETUP_BLUETOOTH: {  // state 3
            bluedar::led::toggleOff();
            bluedar::bt::setupAll();
            appState = BluedarState::SCANNING;
        }

        case BluedarState::SCANNING: {  // state 4
            bluedar::led::toggleOn();
            bluedar::bt::scan(scanPeriodSeconds);
            bluedar::led::toggleOff();
            // TODO publish scan results to mqtt 'discovery' topic

#ifdef SERIAL_DEBUG
            bluedar::bt::printDiscoveredDevices();
#elif defined(MQTT_DEBUG)
            // publish the entire discovered devices map to mqtt 'debug' topic
#endif
        }
    }
}
