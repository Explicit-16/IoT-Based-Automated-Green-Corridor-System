# Known limitations and future work

These are the limitations of my current ESP32 controller implementation.

| Current behavior | Consequence | Possible next step |
|---|---|---|
| Callback sets priority before validating payload | Invalid messages can activate the last selected axis | Validate exact topic and payload before modifying state |
| Wi-Fi setup and MQTT reconnect use blocking loops | Startup can wait indefinitely; MQTT loss freezes the previous signal outputs | Separate connectivity and signal state machines; define loss-of-link behavior |
| All-yellow is applied to all four approaches | Simplified transition differs from a real signal controller | Model per-approach clearance with a timed all-red stage |
| Normal phase retained after priority | Return can jump to a different axis without a clearance stage | Explicit transition state for exiting priority |
| Each command resets priority timer | Command bursts can repeatedly restart yellow | Add command IDs, TTL and refresh/arbitration rules |
| Fixed 1 s yellow + 10 s priority green | No vehicle-passage feedback or explicit cancellation | Define a passage/clear protocol and timeout policy |
| Plain unauthenticated MQTT | Devices on the broker can issue commands | Add authentication, authorization and TLS |
| Single topic/client ID unless configured per node | Shared identifiers can cause interference | Assign unique junction identities |
| No subscription-result check or acknowledgement | A connected log does not prove command delivery | Check subscription result and publish controller state |
| No GPS, route or proximity logic on device | Controller cannot decide which vehicle is nearby | Integrate with the actual central server when source is available |

No timed all-red interlock exists between normal phases. `allRed()` is called at startup only. No FreeRTOS tasks, sensors, relays, dashboards, route optimization implementation or multi-junction synchronization are present in this sketch.

The improvements listed here are **not yet implemented**. I have not included measured travel-time results or claimed road deployment.
