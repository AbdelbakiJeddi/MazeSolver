#include "Arduino.h"
#include "robot.h"

int TICKS_PER_CELL = 1000;
int BASE_SPEED = 120;
float KP_STRAIGHT = 3.0;
int TICKS_PER_90DEG_TURN = 80;  // Calibrate this!


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

    // Ultrasonic sensor pins
    pinMode(LEFT_TRIG, OUTPUT);
    pinMode(LEFT_ECHO, INPUT);
    pinMode(RIGHT_TRIG, OUTPUT);
    pinMode(RIGHT_ECHO, INPUT);
    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);

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
    int correction = error * KP_STRAIGHT;
    int leftSpeed  = baseSpeed - correction;
    int rightSpeed = baseSpeed + correction;
    motorLeft(leftSpeed);
    motorRight(rightSpeed);
}

// ============================================================================
// PID WALL CENTERING
// ============================================================================

float KP_CENTER = 2.0;
float KD_CENTER = 1.0;
float KI_CENTER = 0.5;

int lastCenterError = 0;
float centerIntegral = 0;

void driveCentered(int baseSpeed) {
    int leftDist = readDistanceLeft();
    int rightDist = readDistanceRight();
    
    int error = 0;
    
    if (leftDist < WALL_THRESHOLD * 3 && rightDist < WALL_THRESHOLD * 3) {
        error = leftDist - rightDist;
    } else if (leftDist < WALL_THRESHOLD * 3) {
        error = leftDist - 5;
    } else if (rightDist < WALL_THRESHOLD * 3) {
        error = 5 - rightDist;
    }
    
    centerIntegral += error;
    int derivative = error - lastCenterError;
    lastCenterError = error;
    
    int correction = (KP_CENTER * error) + (KI_CENTER * centerIntegral) + (KD_CENTER * derivative);
    correction = constrain(correction, -30, 30);
    
    int leftSpeed = baseSpeed + correction;
    int rightSpeed = baseSpeed - correction;
    
    leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
    
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

// ============================================================================
// PID TURN FUNCTIONS
// ============================================================================

float KP_TURN = 0.5;
float KD_TURN = 0.3;
float KI_TURN = 2.0;

void turnLeft90PID() {
    resetEncoders();
    int lastError = 0;
    float integral = 0;
    
    while (true) {
        int currentDiff = getRightTicks() - getLeftTicks();
        int error = TICKS_PER_90DEG_TURN - currentDiff;
        
        if (error <= 0) break;
        
        integral += error;
        int derivative = error - lastError;
        float pidOutput = (KP_TURN * error) + (KI_TURN * integral) + (KD_TURN * derivative);
        lastError = error;
        
        int speed = constrain((int)pidOutput, 60, 100);
        
        motorLeft(-speed);
        motorRight(speed);
        
        delay(5);
    }
    printEncoderStatus();
    motorStop();
}

void turnRight90PID() {
    resetEncoders();
    int lastError = 0;
    float integral = 0;
    
    while (true) {
        int currentDiff = getLeftTicks() - getRightTicks();
        int error = TICKS_PER_90DEG_TURN - currentDiff;
        
        if (error <= 0) break;
        
        integral += error;
        int derivative = error - lastError;
        float pidOutput = (KP_TURN * error) + (KI_TURN * integral) + (KD_TURN * derivative);
        lastError = error;
        
        int speed = constrain((int)pidOutput, 60, 100);
        
        motorLeft(speed);
        motorRight(-speed);
        
        delay(5);
    }
    printEncoderStatus();
    motorStop();
}

// ============================================================================
// ULTRASONIC SENSOR FUNCTIONS
// ============================================================================

int readDistanceLeft() {
    digitalWrite(LEFT_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(LEFT_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(LEFT_TRIG, LOW);
    long duration = pulseIn(LEFT_ECHO, HIGH);
    return duration * 0.034 / 2;
}

int readDistanceRight() {
    digitalWrite(RIGHT_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(RIGHT_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(RIGHT_TRIG, LOW);
    long duration = pulseIn(RIGHT_ECHO, HIGH);
    return duration * 0.034 / 2;
}

int readDistanceFront() {
    digitalWrite(FRONT_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(FRONT_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(FRONT_TRIG, LOW);
    long duration = pulseIn(FRONT_ECHO, HIGH);
    return duration * 0.034 / 2;
}

bool hasWallLeft() {
    return readDistanceLeft() < WALL_THRESHOLD;
}

bool hasWallRight() {
    return readDistanceRight() < WALL_THRESHOLD;
}

bool hasWallFront() {
    return readDistanceFront() < WALL_THRESHOLD;
}
