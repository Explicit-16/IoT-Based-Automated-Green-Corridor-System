# MQTT interface and bench demonstration

The broker and topic are configured in the local `config.h`. The example topic is `eshan/green-corridor/junction_1`.

| Payload | Meaning | Output sequence |
|---|---|---|
| `NS` | North–South priority | All-yellow 1 s, NS green / EW red 10 s, normal cycle resumes |
| `EW` | East–West priority | All-yellow 1 s, EW green / NS red 10 s, normal cycle resumes |

Payloads are exact, uppercase ASCII strings, with no newline or surrounding spaces. Do not use JSON. A publisher must send to the same broker and topic as the ESP32. Normal cycling starts only after successful initial MQTT connection.

## Manual commands

Install the Mosquitto command-line clients using the [official downloads](https://mosquitto.org/download/). In the commands below, replace the example IP and topic with your configured values. Run the commands individually in a terminal:

```sh
mosquitto_pub -h 192.168.1.100 -p 1883 -t eshan/green-corridor/junction_1 -m NS
```

Wait at least 12 seconds, observe normal cycling, then:

```sh
mosquitto_pub -h 192.168.1.100 -p 1883 -t eshan/green-corridor/junction_1 -m EW
```

Do not use `-r` (retained messages): a retained command can retrigger when the controller resubscribes. These commands demonstrate the controller independently; they do not implement ambulance tracking or the central server.

## Important actual behavior

- Every received message enters priority mode before payload validation.
- Invalid payloads reuse the previous direction (initially NS), so they can trigger priority.
- Each new message restarts the 11-second priority sequence, including its yellow interval.
- Repeated commands less than one second apart can keep restarting yellow.
- At priority timeout, `lastChange` resets but the previous normal `phase` is retained. The next loop resumes that phase.
- There is no explicit `CLEAR` / passage / cancel message, acknowledgement, command ID or arbitration between vehicles.

See [LIMITATIONS.md](LIMITATIONS.md) for proposed improvements. These are the current controller behaviors; the improvements are not yet implemented.
