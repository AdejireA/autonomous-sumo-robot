# Hardware and Connections

This pin map is taken directly from `autonomous-sumo-robot.ino` and has been checked line by line against the sketch. It describes **firmware pin assignments**; physical wiring beyond what is listed as confirmed below should still be checked against the actual robot.

## Hardware inventory

| Component | Details |
| --- | --- |
| Arduino Uno | 1 |
| L298N motor driver | 1 |
| DC motors | 4, on a prebuilt 4WD chassis, wired as a left pair and a right pair |
| Chassis | Prebuilt 4WD chassis |
| IR boundary sensors | 4 (front-left, front-right, back-left, back-right), inferred from sketch inputs |
| Ultrasonic sensors | 2 (front and rear), inferred from front/rear trigger and echo pins |
| Batteries | Four 18650 lithium-ion cells, 3.7 V nominal each, in a 2S2P arrangement (7.4 V nominal pack voltage) |

## Arduino pin assignments

| Function | Sketch name | Arduino pin |
| --- | --- | --- |
| Front-left IR | `FL` | A0 |
| Front-right IR | `FR` | A1 |
| Back-left IR | `BL` | A2 |
| Back-right IR | `BR` | A3 |
| Motor direction | `in1` | D11 |
| Motor direction | `in2` | D10 |
| Motor direction | `in3` | D9 |
| Motor direction | `in4` | D8 |
| Motor enable / PWM | `ena` | D5 |
| Motor enable / PWM | `enb` | D6 |
| Front ultrasonic trigger | `trigFront` | D13 |
| Front ultrasonic echo | `echoFront` | D12 |
| Rear ultrasonic trigger | `trigBack` | D7 |
| Rear ultrasonic echo | `echoBack` | D4 |

## Motor wiring

The sketch drives two motor-driver channels: `ena`/`in1`/`in2` and `enb`/`in3`/`in4`. The two motors on the left side of the chassis are wired together to one channel, and the two motors on the right side are wired together to the other channel — each L298N channel drives one side of the robot, not one individual motor. This matches `moveForward`/`moveBackward`/`rotateRight`, which always set both channels to the same speed and set directions per-channel (side) rather than per-motor.

## Battery arrangement

The robot is powered by four 18650 lithium-ion cells (3.7 V nominal each) arranged 2S2P: two cells in series to reach 7.4 V nominal, duplicated in a second parallel string for extra capacity. This gives a 7.4 V nominal pack feeding the L298N, which in turn supplies the motors and (via its onboard 5 V regulator, if enabled) potentially the logic side.

**Unverified:** battery protection circuitry (e.g. a BMS or individual cell protection), the charging method/charger used, and whether the Arduino is powered from the L298N's 5 V regulator or from a separate supply. Do not assume the pack is protected or that any particular charging setup is in use without checking the physical hardware.

## Wiring details still to verify

- IR sensor model and output type (the sketch expects analog readings)
- Ultrasonic sensor model and voltage requirements
- Battery protection circuitry and charging method (explicitly unverified, see above)
- L298N power input, logic supply, enable jumpers, and common-ground connections
- Whether the Arduino is powered separately or from the L298N's regulated 5 V output

Do not connect the pack to the Arduino or motor driver solely on the basis of the nominal voltages above — verify the actual pack voltage and wiring condition first.
