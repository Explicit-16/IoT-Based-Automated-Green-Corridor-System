# Validation and evidence

## Current test status

| Item | Status |
|---|---|
| Host tests for controller logic | Passing; see validation-results.txt |
| ESP32 hardware operation | Demonstrated on the project hardware; exact tool versions not logged |
| Hardware observations | Four signal modules and controller logs shown in the project media |
| End-to-end GPS → server → MQTT → controller sequence | Successfully tested using live phone GPS and server-generated commands |
| Project photos and videos | Two photos and two full-duration MP4 recordings included; see DEMO.md |
| Travel-time measurements | No measured results included |

## Integrated demonstration result

We tested live phone GPS → central server → MQTT → ESP32 → traffic-signal modules successfully. A slight delay occurred due to the network connection. I did not measure this delay numerically, so I do not claim a specific response time or travel-time improvement.

The [project media](DEMO.md) show the hardware and controller logs. The Serial Monitor photo captures an NS command and the corresponding priority sequence. The successful upstream GPS/server integration is part of our project test; the recordings focus on the controller rather than showing every stage on screen.

The automated results below cover host-side logic only. They do not represent a fresh compile or flash of this repository version against the ESP32 toolchain.

## Automated tests

From the repository root, run:

```sh
python3 tests/run_tests.py
```

Requires Python 3 and `g++` with C++17 support. The test harness includes the controller `.ino` and replaces Arduino, Wi-Fi and PubSubClient interfaces with minimal mocks. It checks output states and timing boundaries, MQTT configuration, return to normal, and known command-handling behavior. It does not model real networking or Arduino preprocessing and cannot substitute for an ESP32 build.

The workflow in `.github/workflows/host-tests.yml` runs these host tests on GitHub pushes and pull requests. Its first remote run remains unverified until the repository is uploaded.

## Bench test checklist

Record results after running on your hardware; do not mark a case passed from this checklist alone.

| Case | Procedure | Expected from current code | Result |
|---|---|---|---|
| Startup | Power on | All-red for 3 s, remains red while waiting for initial connection | Not recorded |
| Normal NS | Observe first connected phase | NS green, EW red for 10 s | Not recorded |
| Normal transition | Wait after NS | All-yellow for 2 s | Not recorded |
| Normal EW | Continue observing | EW green, NS red for 10 s | Not recorded |
| NS priority | Publish `NS` | 1 s all-yellow, then 10 s NS green | Not recorded |
| EW priority | Publish `EW` | 1 s all-yellow, then 10 s EW green | Not recorded |
| Priority expiry | Stop publishing and wait | Previous normal phase restarts; inspect exit transition | Not recorded |
| Malformed message | Publish `BAD` on isolated bench | Known defect: enters priority using last direction | Not recorded |
| Repeated messages | Publish repeatedly faster than 1 s | Known defect: restarts yellow interval | Not recorded |
| Broker loss | Disconnect broker during operation | Known defect: outputs freeze during reconnect | Not recorded |
| Client ID collision | Connect another node with same ID | Possible disconnects; use unique IDs | Not recorded |

## Record after hardware testing

- Board model: DOIT ESP32 DEVKIT V1 shown in Serial Monitor; board revision not logged.
- Arduino IDE version:
- ESP32 core version:
- PubSubClient version:
- Module model / supply / input polarity:
- Broker and version:
- Date and tester:
- Test observation: integrated live GPS/server sequence successful; slight network delay.
- Photo / video links:

## Recorded media

See [DEMO.md](DEMO.md) for both videos and the photos. The separate bench matrix above remains a repeatable regression checklist; a successful overall demonstration does not mark every failure-injection case as tested.
