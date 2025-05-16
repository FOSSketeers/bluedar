# bluedar-firmware
Development documentation ahead, proceed with caution. Here be dragons...

## State Machine Overview
### 0. Device Powered On
- Go to **state 1**
### 1. Connect to WiFi
- Wait until WiFi connection
- LED = fast blink
- Go to **state 2** when successful

### 2. Set up NTP and Connect to the MQTT Broker
- Wait until both connections
- LED = slow blink
- Go to **state 3** when successful
    
### 3. Set up BT Classic and BLE Scanners
- LED = off
- Go to **state 4** when completed

### 4. Start Scanning (infinite loop)
- LED = on during scanning; off during transmission/non-scanning
- Serialize and publish the discovery data (async)
- Should always stay in this state, unless disconnected from WiFi
- WiFi disconnections are handled with async events
- Go to **state 1** when disconnected from WiFi

## Build Modes (PlatformIO Environments)
### SERIAL_DEBUG
- Prints debug messages to the serial
- Stores the discovered devices and their TTLs in a map and prints them to the serial
### MQTT_DEBUG
- Prints debug messages to the serial and the MQTT `debug` topic
- Stores the discovered devices and their TTLs in a map and publishes them to the MQTT `debug` topic
### RELEASE
- No debug messages printed anywhere
- Discovered devices are only stored until they are published to MQTT `discovery` topic

## MQTT Packets
- Only the newest discoveries are sent, not the entire state with TTLs
- Server side should store the set of discovered devices and their TTLs/expiry dates
- Packet payloads are in BSON (binary JSON) format

### Payload Format
```ts
{
    "probe_id": number // values [1,4]; 0 means invalid or error
    "time": number // unix epoch
    "discovered_devices": array = [
        {
            "is_ble": bool // either ble or bt classic (br/edr)
            "address": string // colon-separated 6-octet device address, should be unique
            "address_type_id": number // values [0,5]: invalid, btclassic, public, random_static, random_private_resolvable, random_private_nonresolvable
            "name": string // device name
            "rssi": number // int8_t
            "appearance_id": number // uint16_t for ble, uint32_t for btc. different lookup tables for each
        },
        {...another device...}, ...
    ]
}
```

