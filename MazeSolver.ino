#include "robot.h"

void setup() {
    Serial.begin(115200);
    robotInit();
    printHelp();
}

void printHelp() {
    Serial.println("=== MazeSolver Tuning Commands ===");
    Serial.println("l - Turn left 90");
    Serial.println("r - Turn right 90");
    Serial.println("e - Encoder status");
    Serial.println("x - Reset encoders");
    Serial.println("p - Print parameters");
    Serial.println("KP<value> - Set turn Kp (e.g. KP0.5)");
    Serial.println("KD<value> - Set turn Kd (e.g. KD0.3)");
    Serial.println("KI<value> - Set turn Ki (e.g. KI2.0)");
    Serial.println("TICKS<value> - Set ticks/90deg (e.g. TICKS160)");
    Serial.println("SPEED<value> - Set base speed (e.g. SPEED120)");
    Serial.println("KPS<value> - Set straight Kp (e.g. KPS3.0)");
    Serial.println("? - Show this help");
    Serial.println("==================================");
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        input.toUpperCase();
        
        if (input == "L") {
            Serial.println("Turning left...");
            turnLeft90PID();
            Serial.println("Done");
        }
        else if (input == "R") {
            Serial.println("Turning right...");
            turnRight90PID();
            Serial.println("Done");
        }
        else if (input == "E") {
            printEncoderStatus();
        }
        else if (input == "X") {
            resetEncoders();
            Serial.println("Encoders reset");
        }
        else if (input == "P") {
            printParams();
        }
        else if (input == "?") {
            printHelp();
        }
        else if (input.startsWith("KP")) {
            float val = input.substring(2).toFloat();
            setKpTurn(val);
            Serial.print("KP_TURN set to: "); Serial.println(val);
        }
        else if (input.startsWith("KD")) {
            float val = input.substring(2).toFloat();
            setKdTurn(val);
            Serial.print("KD_TURN set to: "); Serial.println(val);
        }
        else if (input.startsWith("KI")) {
            float val = input.substring(2).toFloat();
            setKiTurn(val);
            Serial.print("KI_TURN set to: "); Serial.println(val);
        }
        else if (input.startsWith("TICKS")) {
            int val = input.substring(5).toInt();
            setTicksPer90(val);
            Serial.print("TICKS_PER_90DEG set to: "); Serial.println(val);
        }
        else if (input.startsWith("SPEED")) {
            int val = input.substring(5).toInt();
            setBaseSpeed(val);
            Serial.print("BASE_SPEED set to: "); Serial.println(val);
        }
        else if (input.startsWith("KPS")) {
            float val = input.substring(3).toFloat();
            setKpStraight(val);
            Serial.print("KP_STRAIGHT set to: "); Serial.println(val);
        }
        else {
            Serial.print("Unknown command: "); Serial.println(input);
            printHelp();
        }
    }
}