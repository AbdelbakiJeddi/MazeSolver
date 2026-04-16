# Micromouse Maze Solver - Project Overview

## Current Status
Robot can turn 90° accurately using encoder-based PID control.

### What's Working
- Motor control with encoder feedback
- PID-based turning (left/right 90°)
- Encoder tick tracking for wheel rotation

### Key Calibration Values
- `TICKS_FOR_90DEG = 160` (encoder ticks for 90° turn)
- `KP_TURN = 1.0` (PID proportional gain)
- Speed range: 60-150

---

## Principal Navigation Logic

### Overview
The robot navigates the maze using a combination of sensor input and floodfill algorithm:

```
Sensors → Wall Detection → Update Maze Map → Floodfill → Next Move → Execute
```

### 1. Wall Detection (Ultrasonic Sensors)
Three sensors detect walls around the robot:
- **Front sensor**: Detects wall ahead
- **Left sensor**: Detects wall on left
- **Right sensor**: Detects wall on right

Each reading updates the maze map for the current cell.

### 2. Maze Representation
- 16×16 grid representing the maze
- Each cell stores which walls exist (N, E, S, W)
- Robot tracks its current position (x, y) and facing direction

### 3. Floodfill Algorithm
- Calculates distance from every cell to the goal (center)
- Updates when new walls are discovered
- Always finds shortest path to goal

### 4. Decision Making
Based on floodfill distances, robot chooses:
- Turn to cell with lowest distance value
- Move forward if already facing lowest cell
- Turn around if blocked

### 5. Motion Execution
- **Turn Left/Right**: Use PID-controlled 90° turn
- **Move Forward**: Drive straight using encoder feedback
- **Turn Around**: Two 90° turns or one 180° spin

### Integration Flow

```
while not at goal:
    1. Scan walls with ultrasonic sensors
    2. Update maze map with wall data
    3. Run floodfill from current position
    4. Decide next move (turn/move)
    5. Execute movement
    6. Update position after movement
```

---

## Files Structure

| File | Purpose |
|------|---------|
| `robot.h/cpp` | Motors, encoders, PID turning |
| `sensors.h/cpp` | Ultrasonic sensor readings |
| `maze.h/cpp` | Maze grid and floodfill algorithm |
| `MazeSolver.ino` | Main navigation loop |
