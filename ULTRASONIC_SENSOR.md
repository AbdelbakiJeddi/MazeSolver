# Ultrasonic Sensor Module

## Purpose
Detect walls around the robot (front, left, right).

## Sensor Description
HC-SR04 ultrasonic sensor measures distance using sound waves:
- Send 10μs pulse on TRIG pin
- Sensor sends 8 ultrasonic bursts
- ECHO pin goes HIGH until sound returns
- Distance = (ECHO duration × 0.034) / 2

Three sensors needed: front, left, right facing.

## Required Functions

```cpp
void initSensors();           // Initialize pins
int readFrontSensor();        // Return distance in mm
int readLeftSensor();
int readRightSensor();
bool isWallFront();           // True if wall detected
bool isWallLeft();
bool isWallRight();
```

## Notes
- Use appropriate pin numbers
- Set wall detection threshold (e.g., 100mm)
- Handle sensor timing carefully (timeout if no echo)
- Avoid interference between sensors (add small delay between readings)

## Deliverables
`sensors.h` and `sensors.cpp`
