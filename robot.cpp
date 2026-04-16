#include "Arduino.h"
#include "robot.h"

// ============================================================================
// CALIBRATION VARIABLES
// ============================================================================
int TICKS_PER_CELL = 1000;
int BASE_SPEED = 120;
float KP_STRAIGHT = 3.0;
int TICKS_PER_90DEG_TURN = 80;  // Calibrate this!

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================

// Encoder tick counts (volatile for ISR access)
volatile long leftTicks = 0;
volatile long rightTicks = 0;

// Target ticks for movement
long leftTarget = 0;
long rightTarget = 0;

// ============================================================================
// INITIALIZATION
// ============================================================================

void robotInit() {
    // Motor pins (2-pin control per motor)
    pinMode(LeftF, OUTPUT);
    pinMode(LeftR, OUTPUT);
    pinMode(RightF, OUTPUT);
    pinMode(RightR, OUTPUT);

    // Encoder pins
    pinMode(ENC_LEFT_A, INPUT);
    pinMode(ENC_LEFT_B, INPUT);
    pinMode(ENC_RIGHT_A, INPUT);
    pinMode(ENC_RIGHT_B, INPUT);

    // Encoder interrupts
    attachInterrupt(digitalPinToInterrupt(ENC_LEFT_A), leftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_RIGHT_A), rightEncoderISR, CHANGE);

    // Ensure motors are stopped
    motorStop();
}

void resetEncoders() {
    noInterrupts();
    leftTicks = 0;
    rightTicks = 0;
    interrupts();
}


// ============================================================================
// MOTOR CONTROL (2-PIN H-BRIDGE)
// ============================================================================

void motorLeft(int speed) {
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);

    if (speed > 0) {
        // Forward
        analogWrite(LeftF, speed);
        analogWrite(LeftR, 0);
    } 
    else if (speed < 0) {
        // Reverse
        analogWrite(LeftF, 0);
        analogWrite(LeftR, -speed);
    } 
    else {
        // Stop (coast)
        analogWrite(LeftF, 0);
        analogWrite(LeftR, 0);
    }
}

void motorRight(int speed) {
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);

    if (speed > 0) {
        // Forward
        analogWrite(RightF, speed);
        analogWrite(RightR, 0);
    } 
    else if (speed < 0) {
        // Reverse
        analogWrite(RightF, 0);
        analogWrite(RightR, -speed);
    } 
    else {
        // Stop (coast)
        analogWrite(RightF, 0);
        analogWrite(RightR, 0);
    }
}

void motorStop() {
    analogWrite(LeftF, 0);
    analogWrite(LeftR, 0);
    analogWrite(RightF, 0);
    analogWrite(RightR, 0);
}


void driveStraight(int baseSpeed) {
    long error = getLeftTicks() - getRightTicks();

    int correction = error * KP_STRAIGHT;  // gain (tune this)

    int leftSpeed  = baseSpeed - correction;
    int rightSpeed = baseSpeed + correction;

    motorLeft(leftSpeed);
    motorRight(rightSpeed);
}

// ============================================================================
// ENCODER INTERRUPTS
// ============================================================================

void leftEncoderISR() {
    if (digitalRead(ENC_LEFT_B) == digitalRead(ENC_LEFT_A)) {
        leftTicks--;
    } else {
        leftTicks++;
    }
}

void rightEncoderISR() {
    if (digitalRead(ENC_RIGHT_B) == digitalRead(ENC_RIGHT_A)) {
        rightTicks++;
    } else {
        rightTicks--;
    }
}


// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

long getLeftTicks() {
    noInterrupts();
    long ticks = leftTicks;
    interrupts();
    return ticks;
}

long getRightTicks() {
    noInterrupts();
    long ticks = rightTicks;
    interrupts();
    return ticks;
}

void printEncoderStatus() {
    int error = getLeftTicks()-getRightTicks();
    Serial.print(getLeftTicks());
    Serial.print(" ");
    Serial.print(getRightTicks());
    Serial.print(" ");
    Serial.println(error);
}

void setTargetGain(float kp) {
    // This function can be used to update the KP_STRAIGHT gain at runtime if needed.
    // However, since KP_STRAIGHT is defined as a macro, we would need to change it to a variable to allow this.
    // For now, this is just a placeholder to show where you would implement dynamic gain adjustment.
}

// ============================================================================
// TURN FUNCTIONS
// ============================================================================

void turnLeft(int speed) {
    // Left motor backward, right motor forward = turn left
    motorLeft(-speed);
    motorRight(speed);
}

void turnRight(int speed) {
    // Left motor forward, right motor backward = turn right
    motorLeft(speed);
    motorRight(-speed);
}