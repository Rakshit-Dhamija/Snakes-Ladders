# Snakes and Ladders Game

A 2D implementation of the classic Snakes and Ladders board game in C++.

## Features

- Complete implementation of the Snakes and Ladders game
- Two versions:
  - OpenGL/FreeGLUT graphics version (2D visualization)
  - Console text-based version (for environments without OpenGL support)
- Turn-based gameplay for 2 players
- Animated player movements
- Proper snake and ladder mechanics
- Dice rolling
- Win detection

## How to Play

Run the play script to choose which version to play:

```bash
./play.sh
```

### Game Rules

1. Players take turns rolling a die (press Enter or Space)
2. The token moves forward by the number rolled
3. If a player lands on the bottom of a ladder, they climb up to the top
4. If a player lands on the head of a snake, they slide down to the tail
5. First player to reach position 100 exactly wins
6. If a roll would take you beyond position 100, your turn is skipped

### Controls

#### OpenGL Version:
- Press SPACE to roll the dice
- Press R to restart the game

#### Console Version:
- Press ENTER to roll the dice
- Type 'q' to quit the game

## Code Structure

- `main.cpp` - OpenGL implementation using FreeGLUT
- `snakes_ladders_console.cpp` - Text-based implementation
- `Makefile` - Compilation instructions
- `play.sh` - Helper script to choose and run game versions

## Building from Source

To compile both versions:

```bash
make clean
make all
```

Or use the play script option 3 to recompile.

## Requirements

- C++ compiler (g++)
- OpenGL libraries (for graphical version)
- FreeGLUT libraries (for graphical version)

The console version has no external dependencies beyond a standard C++ compiler.

## Technical Details

The game board is represented as a 10x10 grid with a zigzag pattern (1-10 from left to right, 11-20 from right to left, etc.) Snakes and ladders are implemented as a "jump table" that maps source squares to destination squares.

Animation is implemented using timer callbacks to create smooth movement from square to square and for special effects when landing on snakes or ladders.