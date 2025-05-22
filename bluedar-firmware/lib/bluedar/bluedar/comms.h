#pragma once

#ifdef MQTT_DEBUG
#define debug(x) comms::publish(BLUEDAR_MQTT_TOPIC_DEBUG, x);
#elif defined(SERIAL_DEBUG)
#define debug(x) Serial.print(x);
#else
#define debug(x) (void)x;
#endif

#include <ArduinoJson.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "bluedar-config.h"

namespace bluedar::comms {
// NTP related variables
static WiFiUDP _ntpUdp;
static NTPClient timeClient(_ntpUdp);

// MQTT related variables
static WiFiClient _wific;
static PubSubClient mqtt(_wific);

void syncClock();
void setupMqtt();
bool publish(const std::string& topic, const std::string& message);
bool publish(const JsonDocument& payload);
}  // namespace bluedar::comms