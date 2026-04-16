/*
 * MazeSolver - Turn using PID control on encoder difference
 */

#include "robot.h"

// Calibration: encoder tick difference needed for 90 degree spin turn
int TICKS_FOR_90DEG = 160;  // Calibrate this!

// PID constants for turning
float KP_TURN = 0.5;
float KD_TURN = 0.3;
float KI_TURN = 2.0;

void turnLeft90PID();
void turnRight90PID();

void setup() {
    Serial.begin(115200);
    robotInit();
    Serial.println("Commands: l (left), r (right), e (status), x (reset)");
}

void loop() {
    // if (Serial.available() > 0) {
    //     char cmd = Serial.read();
        
    //     if (cmd == 'l') {
    //         Serial.println("Turning left...");
    //         turnLeft90PID();
    //         Serial.println("Done");
    //     }
    //     else if (cmd == 'r') {
    //         Serial.println("Turning right...");
    //         turnRight90PID();
    //         Serial.println("Done");
    //     }
    //     else if (cmd == 'e') {
    //         printEncoderStatus();
    //     }
    //     else if (cmd == 'x') {
    //         resetEncoders();
    //         Serial.println("Reset");
    //     }
    // }
    turnLeft90PID();
    delay(1000);
}

// Turn left: left backward, right forward
void turnLeft90PID() {
    resetEncoders();
    int lastError = 0;
    
    while (true) {
        int currentDiff = getRightTicks() - getLeftTicks();
        int error = TICKS_FOR_90DEG - currentDiff;
        
        if (error <= 0) break;
        
        // PID speed calculation
        int derivative = error - lastError;
        float pidOutput = (KP_TURN * error) + (KD_TURN * derivative);
        lastError = error;
        
        // Constrain to motor range
        int speed = constrain((int)pidOutput, 60, 100);
        
        // Turn: left backward, right forward
        motorLeft(-speed);
        motorRight(speed);
        
        delay(5);
    }
    printEncoderStatus();
    motorStop();
}

// Turn right: left forward, right backward
void turnRight90PID() {
    resetEncoders();
    int lastError = 0;
    
    while (true) {
        int currentDiff = getLeftTicks() - getRightTicks();
        int error = TICKS_FOR_90DEG - currentDiff;
        
        if (error <= 0) break;
        
        // PID speed calculation
        int derivative = error - lastError;
        float pidOutput = (KP_TURN * error) + (KD_TURN * derivative);
        lastError = error;
        
        // Constrain to motor range
        int speed = constrain((int)pidOutput, 60, 100);
        
        // Turn: left forward, right backward
        motorLeft(speed);
        motorRight(-speed);
        
        delay(5);
    }
    printEncoderStatus();
    motorStop();
}
