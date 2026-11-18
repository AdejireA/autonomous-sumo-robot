# Autonomous Sumo Robot

An Arduino-based autonomous sumo robot built as a fifth-year university engineering coursework project. The robot searches an arena for an opponent, detects it with front and rear ultrasonic sensors, attacks by driving toward it, and avoids leaving the arena using four IR boundary sensors. It runs on a prebuilt 4WD chassis driven by an L298N motor driver, with no remote control or external computer involved during a match — all decisions are made on-board by the Arduino Uno.

> **Project status:** The robot has been assembled and physically tested. Opponent detection, autonomous movement, and boundary avoidance have all been confirmed working on the physical robot (see [Testing](#testing)). Some wiring and battery-protection details are still unverified — these are flagged explicitly throughout the documentation rather than assumed.

## Coursework context

This project was built and tested as part of a fifth-year university course. The goal was to design, build, and program a small autonomous robot capable of competing in a sumo-style match: staying inside a bounded arena, finding an opponent robot, and pushing it out or forcing it over the boundary, without any human operator during the match itself.

## Hardware

- Arduino Uno microcontroller
- L298N motor driver
- Four DC motors on a prebuilt 4WD chassis, wired as a left pair and a right pair (both motors on a side share one L298N output channel)
- Four IR sensors for boundary detection (front-left, front-right, back-left, back-right)
- Two ultrasonic sensors (front and rear) for opponent detection
- Four 18650 lithium-ion cells (3.7 V nominal each) in a 2S2P arrangement, giving a 7.4 V nominal pack voltage
- Off-white tiled arena surface with a black boundary line, used for IR calibration and testing

Battery protection circuitry (e.g. a BMS) and the charging method are **unverified** — see [Hardware and connections](docs/hardware-and-connections.md).

See [Hardware and connections](docs/hardware-and-connections.md) for the full pin mapping, verified against the firmware.

## How the firmware behaves

The logic below is taken directly from `loop()` in [`autonomous-sumo-robot.ino`](autonomous-sumo-robot.ino), in the order it actually executes.

1. **Boundary check (edge detection):** Every loop iteration reads all four analog IR sensors (`FL`, `FR`, `BL`, `BR`). If either front sensor reads above `blackThreshold`, that counts as a front edge. If either rear sensor reads above `blackThreshold`, that counts as a rear edge. A front edge is checked before a rear edge, so if both were somehow true in the same iteration, the front response takes priority.
2. **Recovery (escape):** On a front edge, the robot hard-brakes and reverses for `escapeTime` (400 ms). On a rear edge, it hard-brakes and drives forward for the same duration. While `escaping` is true, the loop skips straight to checking whether the escape timer has elapsed — IR and ultrasonic readings are not re-evaluated until the escape finishes. The ultrasonic sensor on the side where the edge occurred is also temporarily disabled for `usDisableDuration` (1000 ms) so a wall or arena edge isn't mistaken for an opponent immediately after escaping.
3. **Opponent search and detection:** Once no edge is active and the escape has finished, the firmware reads the front and (if enabled) rear ultrasonic sensors. If the front sensor reports a valid reading below `detectDistance` (15 cm), the robot drives forward at `attackSpeed`. Otherwise, if the rear sensor reports a valid reading below `detectDistance`, it drives backward at `attackSpeed`.
4. **Search:** If no opponent is detected by either ultrasonic sensor, the robot rotates on the spot (`rotateRight`) at `searchSpeed`, continuously scanning for an opponent until one is found or a boundary is reached.

There is no separate "attack" state machine beyond this — "attacking" is simply driving forward or backward at a higher PWM speed than searching, for as long as the ultrasonic sensor keeps reporting the opponent within range.

### Current parameters

| Parameter | Value in firmware |
| --- | --- |
| `blackThreshold` | `300` (analog reading, `>` comparison; tuned for the off-white tile / black boundary used in testing) |
| `detectDistance` | `15` cm |
| `searchSpeed` | `120` (PWM, 0–255) |
| `attackSpeed` | `200` (PWM, 0–255) |
| `reverseSpeed` | `200` (PWM, 0–255) |
| `escapeTime` | `400` ms |
| `usDisableDuration` | `1000` ms |

## Setup and upload guide

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) if not already installed.
2. Connect the Arduino Uno to your computer with a USB cable.
3. Open `autonomous-sumo-robot.ino` in the Arduino IDE. The sketch only uses built-in Arduino core functions — no additional libraries need to be installed.
4. In the IDE, select **Tools > Board > Arduino Uno** and **Tools > Port**, choosing the port the Uno appears on.
5. Click **Upload**. The IDE compiles the sketch and flashes it to the board.
6. With the robot powered and the wheels clear of the ground, confirm motor directions and sensor readings behave as expected before placing it on the arena surface (see [IR sensor calibration](docs/ir-sensor-calibration.md) for the calibration procedure used).

**Safety:** Always test with the wheels lifted off the ground first. Verify motor directions, IR readings, and ultrasonic readings before running the robot in the arena. Do not assume the battery pack and wiring are safe without checking voltages and connections yourself — see the unverified items in [Hardware and connections](docs/hardware-and-connections.md).

## IR sensor calibration

Calibration was done by physically adjusting the potentiometers on the IR sensor modules while watching readings on the off-white tile and the black boundary line, then tuning `blackThreshold` in the firmware to sit cleanly between the two. There is no automatic calibration routine in the sketch. See the [IR sensor calibration guide](docs/ir-sensor-calibration.md) for the full procedure.

## Testing

The assembled robot was physically tested on the off-white tiled arena with a black boundary. Testing confirmed, on the physical robot:

- **Boundary avoidance:** the IR sensors correctly detected the black boundary line and triggered the escape (hard brake + reverse/forward) behavior.
- **Opponent detection:** the front and rear ultrasonic sensors detected a nearby object and triggered the attack (drive-toward) behavior.
- **Autonomous movement:** the robot searched, detected, and reacted to the arena boundary and a nearby object without manual intervention.

No timed runs, competition results, or quantitative performance measurements were recorded, so none are reported here.

## Media

Photographs and a demonstration video of the physical robot are planned but not yet added to this repository. They will be linked here once available, rather than referenced as placeholder files.
