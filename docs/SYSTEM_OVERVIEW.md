# Wider system and controller interface

Our project connects live smartphone location data to an ESP32 traffic-signal controller through a central server and MQTT. My responsibilities were ESP32 programming, wiring and MQTT integration.

## Data flow

| Stage | Responsibility | Relationship to this repository |
|---|---|---|
| Smartphone tracking | Publish current GPS coordinates | External tracking client |
| Central server | Process location, check junction proximity and select NS/EW priority | External backend |
| MQTT broker | Route location updates and controller commands | External service configured in config.h |
| ESP32 controller | Receive NS/EW, run signal phases and timed priority override | Firmware included here |
| Signal modules | Display the GPIO-controlled states | Wiring documented here |

## Backend context

The implementation chapter of our project report describes OwnTracks for smartphone GPS publishing, a Python/FastAPI backend with Paho MQTT, Haversine distance calculations and an HTML/JavaScript dashboard using Leaflet. The original test broker was `broker.hivemq.com` on port 1883; the controller used client ID `junction_1` and topic `traffic/junction_1`.

The report also discusses Google Maps in the wider design. The firmware itself is independent of the phone application and mapping provider: its interface is an MQTT topic carrying `NS` or `EW`.

Junction coordinates, distance thresholds and direction selection belong to the backend. The report describes different proximity thresholds for different test scenarios; no distance threshold is implemented in this ESP32 sketch.

## Controller version documented here

- Traffic lights are GPIO-driven modules, with twelve signal outputs.
- Normal phases last 10 seconds green plus 2 seconds all-yellow.
- Priority lasts 1 second all-yellow plus 10 seconds requested-axis green.
- Priority ends by timer; no passage detector or reset command is implemented.
- The controller does not implement a timed all-red interlock, explicit watchdog recovery policy, or manual override control.

The report includes broader descriptions of relay actuation, reset states and fail-safe features. For the version in this repository, the actual firmware and hardware determine the supported behavior. In particular, sending `RED` is not a supported command; the current callback would treat it as an invalid payload and reuse the previous priority direction.

## Integration result

We successfully tested the live phone GPS → server → MQTT → ESP32 → signal-module chain. A slight network-related delay was observed. No numerical latency or travel-time improvement is claimed here.

## Documentation reference

Technical context: our final-year report, *Green Corridor For Emergency Vehicle*, academic year 2025–26, Chapter 6 (printed pages 27–36; PDF pages 36–45). This repository contains the controller code and project media rather than the full backend or tracking application source.
