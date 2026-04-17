# MazeSolver Project Status

## Git Branches

| Branch | Purpose | Merge to main? |
|--------|---------|----------------|
| `main` | Final competition code | - |
| `feature/bluetooth_tuning` | Tuning/calibration tools via Serial | **No** (keep separate) |
| `feature/ultrasonic-sensors` | Add ultrasonic sensors | **Yes** (TODO) |
| `feature/floodfill` | Floodfill algorithm | **Yes** (TODO) |

## Branch Contents

### main
- `moveOneCell(targetDirection)` - Navigate to target cell
- `turnToDirection()` - Turn logic (0°, 90°, 180°, 270°)
- `moveForwardOneCell()` - Drive straight using encoders
- Direction tracking (`currentDirection`: N/E/S/W)
- PID turn functions with integral term

### feature/bluetooth_tuning
- Serial commands for tuning PID gains
- Commands: `KP`, `KD`, `KI`, `TICKS`, `SPEED`, `KPS`
- `printParams()` - Show all parameters
- HC-05/HC-06 Bluetooth integration (same Serial)

## Bluetooth Connection
- HC-05 TX → Arduino RX (pin 0)
- HC-05 RX → Voltage divider → Arduino TX (pin 1)
- Disconnect pin 0 before uploading via USB

## TODO
- [ ] Implement `hasWallLeft()`, `hasWallRight()`, `hasWallFront()` (ultrasonic)
- [ ] Implement `getNextDirection()` (floodfill)
- [ ] Implement `moveForwardOneCell()` with proper PID

## Git Workflow
```bash
# Work on new features
git checkout main
git checkout -b feature/ultrasonic-sensors
# ... code ...
git add . && git commit -m "Add ultrasonic sensors"
git checkout main
git merge feature/ultrasonic-sensors

# Keep bluetooth_tuning separate for competition tuning
# DON'T merge into main
``

## Last Session
- Added PID integral term to turn functions
- Created `moveOneCell()` with direction tracking
- Aborted merge conflict between main and bluetooth_tuning
- Decided to keep branches separate