# Team Tasks

## Member 1: Ultrasonic Sensor Integration

**Goal**: Add wall detection capability to the robot

### Tasks
1. Connect ultrasonic sensors to Arduino pins
2. Create sensor reading functions:
   - `readFrontSensor()` - returns distance in mm
   - `readLeftSensor()` - returns distance in mm
   - `readRightSensor()` - returns distance in mm
3. Create wall detection functions:
   - `bool isWallFront()` - true if wall detected ahead
   - `bool isWallLeft()` - true if wall on left
   - `bool isWallRight()` - true if wall on right
4. Set threshold distance for wall detection (e.g., 100mm)
5. Test sensor accuracy and response time

### Deliverables
- `sensors.h` - Header file with function declarations
- `sensors.cpp` - Implementation file

### Interface
```cpp
// Read raw distance
int readFrontSensor();
int readLeftSensor();
int readRightSensor();

// Boolean wall detection
bool isWallFront();
bool isWallLeft();
bool isWallRight();
```

---

## Member 2: Floodfill Algorithm

**Goal**: Implement maze solving logic

### Tasks
1. Create 16×16 maze grid data structure
2. Store wall information for each cell (N, E, S, W walls)
3. Implement floodfill algorithm:
   - Calculate distance from goal to all cells
   - Update distances when walls are discovered
4. Track robot state:
   - Current position (x, y)
   - Facing direction (N, E, S, W)
5. Decision function: `getNextMove()` returns action

### Deliverables
- `maze.h` - Maze structure and floodfill declarations
- `maze.cpp` - Maze and floodfill implementation

### Interface
```cpp
// Initialize maze
void initMaze();

// Update walls at current position
void updateWalls(int x, int y, bool front, bool left, bool right);

// Run floodfill from current position
void floodfill(int fromX, int fromY);

// Get next movement decision
// Returns: 'L'=turn left, 'R'=turn right, 'F'=forward, 'B'=turn around
char getNextMove(int currentX, int currentY, char facing);

// Update position after move
void updatePosition(int* x, int* y, char* facing, char move);
```

---

## Integration

Once both members complete their tasks:

```cpp
// In main loop
initMaze();

while (!atGoal(currentX, currentY)) {
    // Member 1: Read sensors
    bool front = isWallFront();
    bool left = isWallLeft();
    bool right = isWallRight();
    
    // Member 2: Update maze and decide
    updateWalls(currentX, currentY, front, left, right);
    floodfill(currentX, currentY);
    char nextMove = getNextMove(currentX, currentY, facing);
    
    // Execute movement (existing robot functions)
    if (nextMove == 'L') turnLeft90PID();
    else if (nextMove == 'R') turnRight90PID();
    else if (nextMove == 'F') moveForward();
    else if (nextMove == 'B') turnAround();
    
    // Update position
    updatePosition(&currentX, &currentY, &facing, nextMove);
}
```
