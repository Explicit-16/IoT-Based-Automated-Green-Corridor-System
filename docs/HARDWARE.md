# Hardware and pin map

## Required bench components

- Classic ESP32 DevKit / ESP32-WROOM-32 board with the GPIOs below exposed.
- Four compatible red/yellow/green traffic-light modules, or twelve LEDs with appropriate current-limiting resistors.
- Breadboard, jumper wires and USB power/data cable.
- 2.4 GHz Wi-Fi and a reachable MQTT broker.

I wired traffic-signal modules to the ESP32. The firmware uses **active-high** outputs. Module model details and a circuit diagram are still to be documented.

| Direction | Red GPIO | Yellow GPIO | Green GPIO |
|---|---:|---:|---:|
| North | 5 | 18 | 19 |
| South | 21 | 22 | 23 |
| East | 13 | 12 | 14 |
| West | 25 | 32 | 33 |

Use a shared ground and verify module supply voltage, input polarity and current requirements against its datasheet. ESP32 GPIO signals are 3.3 V; use an appropriate interface if a module requires higher-voltage control or exceeds GPIO current capability. Never connect road signal loads directly.

GPIO 5 and GPIO 12 have boot-strapping functions on classic ESP32. Check your board's documentation and ensure connected modules do not force incompatible startup levels; GPIO 12 is especially relevant to boot configuration. Remap pins if the module's input loading interferes with startup. See the [ESP32 datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf).

## Output states

| State | North / South | East / West |
|---|---|---|
| Startup | Red | Red |
| NS green | Green | Red |
| EW green | Red | Green |
| All yellow | Yellow | Yellow |

The [prototype photo](media/traffic-signal-prototype.jpg) shows the modules and breadboard wiring. The videos also show the ESP32 board; see [DEMO.md](DEMO.md).
