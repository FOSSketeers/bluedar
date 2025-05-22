#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/* Instructions:
- Rename this file to bluedar-config.h
- Do not change the lines above
- Modify the values below to match your setup
*/

#define BLUEDAR_PROBE_ID 2
#define BLUEDAR_WIFI_SSID "YOUR WIFI SSID"
#define BLUEDAR_WIFI_PASS "YOUR WIFI PASSPHRASE"
#define BLUEDAR_MQTT_SERVER "your.mqtt.broker.com"
#define BLUEDAR_MQTT_PORT 1883
#define BLUEDAR_MQTT_ID ("bluedar-probe-id-" TOSTRING(BLUEDAR_PROBE_ID))
#define BLUEDAR_MQTT_USER ("bluedar-probe-user-" TOSTRING(BLUEDAR_PROBE_ID))
#define BLUEDAR_MQTT_PASS ""
#define BLUEDAR_MQTT_TOPIC "bluedar-topic-discovery"
#define BLUEDAR_MQTT_TOPIC_DEBUG "bluedar-topic-debug"
#define BLUEDAR_SCAN_PERIOD_SECONDS 5
