# Floodfill Algorithm Module

## Purpose
Find shortest path to maze goal (center).

## How It Works

Floodfill assigns a "distance" number to each cell representing how far it is from the goal.

**Step 1**: Set goal cell distance = 0

**Step 2**: Spread outward through open paths (no walls):
- Goal neighbors get distance = 1
- Their neighbors get distance = 2
- Continue until all reachable cells have distances

**Step 3**: Robot follows decreasing numbers to reach goal

## Example

```
Goal at center (distance 0):

    ?   ?   ?   ?   ?
    ?   2   1   2   ?
    ?   1   0   1   ?   <- Goal is 0
    ?   2   1   2   ?
    ?   ?   ?   ?   ?
```

Robot at bottom-left sees neighbor with distance 2, moves there.
Then to 1, then to 0 (goal).

## Wall Handling

When robot detects a wall, block that direction and re-run floodfill.
New path will avoid the wall automatically.

## Required Functions

```cpp
void initMaze();                                    // Setup 16x16 grid
void updateWalls(int x, int y, bool front, bool left, bool right);
void floodfill();                                   // Calculate distances
char getNextMove(int x, int y, char facing);        // Return L/R/F/B
void updatePosition(int* x, int* y, char* facing, char move);
bool isAtGoal(int x, int y);
```

## Data Structure

Store maze as 16x16 grid. Each cell needs:
- Wall info: wallNorth, wallEast, wallSouth, wallWest (booleans)
- Distance: distance to goal (integer)

## Algorithm Steps

1. Initialize all distances to a large number (infinity)
2. Set goal distance = 0, add goal to queue
3. While queue not empty:
   - Take cell from queue
   - For each neighbor without wall:
     - If neighbor distance > current distance + 1:
       - Set neighbor distance = current distance + 1
       - Add neighbor to queue

## Deliverables
`maze.h` and `maze.cpp`
