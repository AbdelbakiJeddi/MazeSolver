#ifndef ROBOT_H 
#define ROBOT_H
#include <Arduino.h>

// PINS
#define LeftF  6
#define LeftR  5
#define RightF 9
#define RightR 10

// ENCODERS
#define ENC_LEFT_A  2
#define ENC_LEFT_B  A4
#define ENC_RIGHT_A 3
#define ENC_RIGHT_B A3

// ULTRASONIC SENSORS (HC-SR04)
#define LEFT_TRIG   7
#define LEFT_ECHO   8
#define RIGHT_TRIG  11
#define RIGHT_ECHO  12
#define FRONT_TRIG  13
#define FRONT_ECHO  A0

#define WALL_THRESHOLD 10  // cm - wall detected if distance < this

#define MAX_SPEED 255
#define MIN_SPEED 50

// CALIBRATION
extern int TICKS_PER_CELL;
extern int BASE_SPEED;
extern float KP_STRAIGHT;
extern int TICKS_PER_90DEG_TURN;

// PID TURN GAINS
extern float KP_TURN;
extern float KD_TURN;
extern float KI_TURN;

// PID WALL CENTERING GAINS
extern float KP_CENTER;
extern float KD_CENTER;
extern float KI_CENTER;

// FUNCTIONS
void robotInit();
void resetEncoders();

void motorLeft(int speed);
void motorRight(int speed);
void motorStop();

void driveStraight(int baseSpeed);
void driveCentered(int baseSpeed);
void turnLeft(int speed);
void turnRight(int speed);

void leftEncoderISR();
void rightEncoderISR();

long getLeftTicks();
long getRightTicks();
void printEncoderStatus();

void turnLeft90PID();
void turnRight90PID();

void setKpTurn(float kp);
void setKdTurn(float kd);
void setKiTurn(float ki);
void setTicksPer90(int ticks);
void setBaseSpeed(int speed);
void setKpStraight(float kp);
void printParams();

// Direction constants
#define DIR_NORTH 0
#define DIR_EAST  1
#define DIR_SOUTH 2
#define DIR_WEST  3

// Movement functions
void moveOneCell(int targetDirection);

// Sensor functions
bool hasWallLeft();
bool hasWallRight();
bool hasWallFront();
int readDistanceLeft();
int readDistanceRight();
int readDistanceFront();

// Floodfill functions (to be implemented)
int getCurrentDirection();
int getNextDirection();

#endif // ROBOT_H