#include <Arduino.h>
#include <bluedar/bluetooth.h>

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
            // TODO led blink fast (async)
            // TODO connect to wifi (sync)
            // TODO wifi disconnect event handler sets appState to 1 (async)
            appState = BluedarState::SETUP_NTP_AND_MQTT;
            break;
        }

        case BluedarState::SETUP_NTP_AND_MQTT: {  // state 2
            // TODO led blink slow (async)
            // TODO set up ntp
            // TODO connect to mqtt
            appState = BluedarState::SETUP_BLUETOOTH;
            break;
        }

        case BluedarState::SETUP_BLUETOOTH: {  // state 3
            // TODO led off
            bluedar::bt::setupAll();
            appState = BluedarState::SCANNING;
        }

        case BluedarState::SCANNING: {  // state 4
            // TODO led on
            bluedar::bt::scan(scanPeriodSeconds);
            // TODO led off
            // TODO publish scan results to mqtt 'discovery' topic

#ifdef SERIAL_DEBUG
            bluedar::bt::printDiscoveredDevices();
#elif defined(MQTT_DEBUG)
            // publish the entire discovered devices map to mqtt 'debug' topic
#endif
        }
    }
}
