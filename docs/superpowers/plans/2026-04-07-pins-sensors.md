# Pins & Sensors Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Create pin configuration and ultrasonic sensor reading module for Arduino maze-solving robot.

**Architecture:** Single config.h for all constants, sensors module with simple HC-SR04 reading functions. Minimal memory footprint for Arduino Uno.

**Tech Stack:** Arduino C++, no external libraries

---

## Task 1: Create config.h

**Files:**
- Create: `config.h`

- [ ] **Create config.h with pin definitions and constants**

```cpp
#ifndef CONFIG_H
#define CONFIG_H

// Encoders
#define ENC_LEFT_A   2
#define ENC_LEFT_B   A4
#define ENC_RIGHT_A  3
#define ENC_RIGHT_B  A3

// Motors
#define LEFT_PWM   6
#define LEFT_IN1   5
#define LEFT_IN2   4
#define RIGHT_PWM  9
#define RIGHT_IN1  7
#define RIGHT_IN2  8

// Sensors (HC-SR04)
#define FRONT_TRIG  10
#define FRONT_ECHO  11
#define LEFT_TRIG   12
#define LEFT_ECHO   13
#define RIGHT_TRIG  A0
#define RIGHT_ECHO  A1

// Calibration
#define TICKS_PER_CELL  2000
#define TICKS_90_DEG    400
#define TICKS_180_DEG   800

// Sensor
#define WALL_THRESHOLD  15.0
#define SENSOR_TIMEOUT  30000

// Serial
#define SERIAL_BAUD 115200

#endif
```

---

## Task 2: Create sensors.h

**Files:**
- Create: `sensors.h`

- [ ] **Create sensors.h with interface**

```cpp
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

struct WallStatus {
    bool front;
    bool left;
    bool right;
};

void sensorsInit();
float readDistance(int trigPin, int echoPin);
WallStatus getWallStatus();

#endif
```

---

## Task 3: Create sensors.cpp

**Files:**
- Create: `sensors.cpp`

- [ ] **Create sensors.cpp with implementation**

```cpp
#include "sensors.h"
#include "config.h"

void sensorsInit() {
    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);
    pinMode(LEFT_TRIG, OUTPUT);
    pinMode(LEFT_ECHO, INPUT);
    pinMode(RIGHT_TRIG, OUTPUT);
    pinMode(RIGHT_ECHO, INPUT);

    digitalWrite(FRONT_TRIG, LOW);
    digitalWrite(LEFT_TRIG, LOW);
    digitalWrite(RIGHT_TRIG, LOW);
}

float readDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH, SENSOR_TIMEOUT);

    if (duration == 0) return -1;

    return (duration / 2.0) * 0.0343;
}

WallStatus getWallStatus() {
    WallStatus w;

    float f = readDistance(FRONT_TRIG, FRONT_ECHO);
    delayMicroseconds(100);
    float l = readDistance(LEFT_TRIG, LEFT_ECHO);
    delayMicroseconds(100);
    float r = readDistance(RIGHT_TRIG, RIGHT_ECHO);

    w.front = (f > 0 && f < WALL_THRESHOLD);
    w.left  = (l > 0 && l < WALL_THRESHOLD);
    w.right = (r > 0 && r < WALL_THRESHOLD);

    return w;
}
```

---

## Task 4: Create minimal MazeSolver.ino

**Files:**
- Create: `MazeSolver.ino`

- [ ] **Create basic sketch**

```cpp
#include "config.h"
#include "sensors.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    sensorsInit();
    Serial.println("Ready. Commands: s=sensors, p=pins");
}

void loop() {
    if (Serial.available()) {
        char c = Serial.read();
        if (c == 's') {
            WallStatus w = getWallStatus();
            Serial.print("Front:");
            Serial.print(w.front ? "Y" : "N");
            Serial.print(" Left:");
            Serial.print(w.left ? "Y" : "N");
            Serial.print(" Right:");
            Serial.println(w.right ? "Y" : "N");
        }
    }
}
```