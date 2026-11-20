#pragma once
// Copy to config.h, fill in locally, and never upload config.h.
#define WIFI_SSID "YOUR_2_4_GHZ_WIFI"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
// Use the LAN address of a broker you control. localhost would mean the ESP32.
#define MQTT_HOST "192.168.1.100"
#define MQTT_PORT 1883
// Give each controller its own client ID and topic.
#define MQTT_CLIENT_ID "eshan_green_corridor_junction_1"
#define MQTT_TOPIC "eshan/green-corridor/junction_1"
