#include "comms.h"

namespace bluedar::comms {

void syncClock() {
    timeClient.update();
}

void setupMqtt() {
    delay(100);
    mqtt.setServer(BLUEDAR_MQTT_SERVER, BLUEDAR_MQTT_PORT);
    mqtt.setKeepAlive(20);
    mqtt.setBufferSize(2048);

    bool success = false;
    while (!success) {
        success =
            mqtt.connect(BLUEDAR_MQTT_ID, BLUEDAR_MQTT_USER, BLUEDAR_MQTT_PASS);
        delay(100);
    }
}

bool publish(const std::string& topic, const std::string& message) {
    if (!mqtt.connected()) {
        setupMqtt();
    }

    return mqtt.publish(topic.c_str(), message.c_str());
}

bool publish(const JsonDocument& payload) {
    if (!mqtt.connected()) {
        setupMqtt();
    }

    char buffer[2048];
    size_t n = serializeJson(payload, buffer, sizeof(buffer));
    return mqtt.publish(BLUEDAR_MQTT_TOPIC, buffer, n);
}

}  // namespace bluedar::comms