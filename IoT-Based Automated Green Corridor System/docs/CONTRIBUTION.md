# My contribution

I worked on the ESP32 traffic-signal controller for our academic project, **IoT-Based Automated Green Corridor for Emergency Vehicles**.

## ESP32 programming

I programmed the controller in Arduino C/C++ to operate the red, yellow and green outputs for four directions. The firmware alternates between North–South and East–West phases during normal operation. When it receives an emergency-priority command, it applies a temporary override and then resumes the normal cycle.

## Hardware wiring

I wired the traffic-signal modules to the ESP32 using twelve GPIO outputs. The pin assignments are documented in [HARDWARE.md](HARDWARE.md). The implementation uses signal modules, rather than relay-based traffic lights.

## MQTT integration

I integrated Wi-Fi and MQTT communication using the ESP32 Wi-Fi library and PubSubClient. The controller subscribes to a junction topic, receives `NS` or `EW` commands, and selects the priority axis.

## Wider project

Our wider system used smartphone GPS, a tracking interface and a central server to identify nearby junctions and send priority commands. My responsibilities covered ESP32 programming, wiring and MQTT integration. This repository contains the controller component; it does not contain the tracking application or the central-server implementation.

## Repository configuration

Wi-Fi and MQTT settings are kept in a local `config.h` so credentials stay out of version control. The example configuration uses a LAN broker address and project-specific topic/client identifiers. These can be changed to match the broker used for a demonstration.

## Integration testing

We tested the controller as part of the live phone GPS and server-command chain. The signal-priority response worked, with a slight delay caused by the network connection. My photos and demonstration videos are linked in [DEMO.md](DEMO.md).
