# Build and run

## 1. Install tools

1. Install [Arduino IDE](https://www.arduino.cc/en/software).
2. Open Preferences and add the stable ESP32 Boards Manager URL:
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. In Boards Manager, install **esp32 by Espressif Systems**.
4. In Library Manager, install **PubSubClient by Nick O'Leary**. `WiFi.h` comes with the ESP32 Arduino core.

Reference: [Espressif installation instructions](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

Record the IDE, board-core and library versions used for your build in `docs/VALIDATION.md`. The original build versions are not yet documented here.

## 2. Configure locally

Copy `firmware/green_corridor/config.example.h` to `firmware/green_corridor/config.h`.
Edit the copied file:

| Setting | Value to enter |
|---|---|
| `WIFI_SSID` | Your 2.4 GHz Wi-Fi name |
| `WIFI_PASSWORD` | Your Wi-Fi password |
| `MQTT_HOST` | Reachable LAN IPv4 address or hostname of your broker |
| `MQTT_PORT` | Broker's plain MQTT port, usually 1883 |
| `MQTT_CLIENT_ID` | Unique ID for this ESP32 |
| `MQTT_TOPIC` | Topic shared with your test publisher / server |

Use an isolated bench network. This sketch has no MQTT username/password or TLS support. The broker must allow this client's connection and topic subscription on that network. Setting username/password fields only in a broker will not add authentication support to this firmware. Never expose an anonymous broker to the internet.

For example, a broker on your laptop might use `192.168.1.100`; replace it with the real address. `localhost` on an ESP32 does not point to your laptop. Broker installation and listener configuration depend on the operating system; follow the [Mosquitto documentation](https://mosquitto.org/documentation/) if using Mosquitto, and allow the ESP32 to reach the listener through the local firewall.

## 3. Verify and upload

1. Wire the modules with power disconnected using [HARDWARE.md](HARDWARE.md).
2. Connect the ESP32 by USB.
3. Open `green_corridor.ino` in its `green_corridor` folder.
4. Select the board matching your hardware (for example, DOIT ESP32 DEVKIT V1 if that is your board) and its serial port.
5. Click **Verify**, then **Upload**.
6. Open Serial Monitor at **115200 baud**.
7. Observe startup all-red, Wi-Fi connection, MQTT connection, then normal traffic cycling.

The sketch remains all-red while waiting for initial Wi-Fi. During MQTT connection attempts it blocks the main loop, leaving the previous outputs unchanged.

## 4. Demonstrate commands

Follow [MQTT_PROTOCOL.md](MQTT_PROTOCOL.md), then complete the hardware test matrix in [VALIDATION.md](VALIDATION.md).

## Troubleshooting

| Symptom | Check |
|---|---|
| `config.h: No such file or directory` | Copy the example header into the sketch folder and rename the copy |
| `PubSubClient.h` missing | Install PubSubClient in Library Manager |
| Wi-Fi dots continue | Wi-Fi name/password, 2.4 GHz availability and hotspot settings |
| MQTT reconnect repeats | Broker LAN address, listener, port, network access and firewall |
| Commands do nothing | Exact topic and uppercase payload, active connection and correct broker |
| ESP32 disconnects when another node connects | Give each device a unique MQTT client ID |
| Board fails to boot with modules connected | Module loading on boot-strapping GPIOs; verify supply and wiring |
| Normal traffic freezes on broker loss | Known blocking reconnect behavior; see LIMITATIONS.md |

## Keep credentials private

`config.h` is excluded by `.gitignore` for Git uploads. Browser file uploads do not apply this rule for you: leave `config.h` out manually. Keep real Wi-Fi values in `config.h`. If credentials have been shared publicly, change them before reuse.
