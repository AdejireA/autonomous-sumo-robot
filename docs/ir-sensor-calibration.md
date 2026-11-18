# IR Boundary Sensor Calibration

## Arena surface

The arena used for testing has an off-white tiled interior with a black boundary line marking the edge. The IR sensors rely on the contrast between these two surfaces: the off-white tile reflects IR more strongly than the black boundary (or vice versa, depending on sensor orientation — see Polarity below), which the sketch uses to detect when the robot is about to leave the arena.

## Why calibration matters

The sketch sets `const int blackThreshold = 300;` and interprets any IR reading **greater than 300** as a boundary detection. This threshold was reached through manual calibration against the actual arena surface, not an automatic routine. Floor/boundary contrast, sensor mounting height, and ambient light all affect the raw readings, so the threshold is specific to this robot and arena setup.

## Calibration procedure used

1. **Make the robot safe.** Wheels lifted clear of the surface, or motor power disconnected, while adjusting sensors and reading values.
2. **Adjust the sensor-module potentiometers.** Each IR sensor module has an onboard potentiometer that sets its switching sensitivity. These were adjusted by hand while watching the sensor's response over the off-white tile and over the black boundary line, at the robot's normal mounting height.
3. **Read each IR sensor in firmware.** `analogRead(A0)` through `analogRead(A3)` (`FL`, `FR`, `BL`, `BR`) were printed to the Arduino Serial Monitor (`Serial.begin(9600)`, matched in the Serial Monitor) to observe actual values over both surfaces.
4. **Check polarity.** With the potentiometers set, the boundary line was confirmed to read **higher** than the tile, matching the `>` comparison used in the firmware's `frontBlack`/`backBlack` checks.
5. **Tune the firmware threshold.** `blackThreshold` was set in the sketch to a value between the typical tile and boundary readings observed, with margin for noise, then re-uploaded.
6. **Retest.** Each corner of the robot was tested individually over the boundary, then the front and rear escape directions were verified at low speed before running full tests on the arena.

## Notes about current firmware behavior

- The firmware triggers a front escape when **either** front sensor (`FL` or `FR`) crosses the threshold, and a rear escape when **either** rear sensor (`BL` or `BR`) crosses it.
- If front and rear boundary conditions are both true in the same loop iteration, the front check takes priority (it is evaluated first in `loop()`).
- While the 400 ms escape (`escapeTime`) is active, the main loop returns early each iteration without re-reading the IR sensors, so the escape always runs to completion once triggered.
- This calibration is manual, tuned specifically for the off-white tile / black boundary arena described above. Re-calibration (new potentiometer adjustment and/or a new `blackThreshold` value) is expected to be needed on a different surface or under different lighting.
