# ⚓ Battleships

A console-based Battleships game for Windows written in C++, featuring a smart AI opponent with hunt/target logic.

---

## Features

- **10×10 grid** with lettered rows (A–J) and numbered columns (1–10)
- **Manual ship placement** — position your fleet before battle
- **Smart AI opponent** — uses a checkerboard hunt strategy and switches to targeted firing once a hit is scored
- **Buffer zones** — ships are automatically separated by one cell, matching classic Battleships rules
- **Win/loss detection** with fleet health tracking

---

## Fleet Composition

| Ship Size | Count |
|-----------|-------|
| 1-cell    | 4     |
| 2-cell    | 3     |
| 3-cell    | 2     |
| 4-cell    | 1     |

---

## Requirements

- **OS:** Windows (uses `<Windows.h>`, `<conio.h>`)
- **Compiler:** MSVC, MinGW, or any Windows-compatible C++ compiler
- **Standard:** C++11 or later

---

## Building

**With g++ (MinGW):**
```bash
g++ -std=c++11 -o Battleship Battleship.cpp
```

**With MSVC:**
```bash
cl /EHsc Battleship.cpp
```

---

## How to Play

### Main Menu
Launch the executable and press:
- **`1`** — Start a new game
- **`2`** — Exit

### Setup Phase
Place your ships before the battle begins.

**Command format:**
```
<coordinate> <length> <direction>
```

| Part        | Format            | Example |
|-------------|-------------------|---------|
| Coordinate  | Letter + number   | `C3`    |
| Length      | 1–4               | `4`     |
| Direction   | `V` or `H`        | `V`     |

**Example — place a 4-cell ship vertically at C3:**
```
C3 4 V
```

**Other setup commands:**
- **`P`** — Done placing, proceed to battle
- **`R`** — Reset all ships and start placement over

Ships cannot overlap or touch each other. Invalid placements are rejected with a message.

### Battle Phase
Take turns firing at the enemy grid.

**Shot format:**
```
<coordinate>
```

**Example:**
```
E7
```

The board updates after each shot:
| Symbol | Meaning       |
|--------|---------------|
| `.`    | Unexplored    |
| `X`    | Hit           |
| `O`    | Miss          |

The **RADAR** shows your shots on the enemy grid. **FLEET STATUS** shows incoming fire on your grid.

**Turn order:**
- If you **hit**, you shoot again
- If you **miss**, the AI takes its turn
- If the AI **hits**, it shoots again immediately

The game ends when all ships on either side are sunk.

---

## AI Behaviour

The enemy AI operates in two modes:

- **Hunt mode** — fires at checkerboard-pattern cells to maximise coverage, then falls back to remaining cells if the pattern is exhausted
- **Target mode** — activates on a hit; probes adjacent cells to find ship orientation, then fires along the axis until the ship is sunk

After sinking a ship, the AI resets to hunt mode.

---

## Project Structure

| Class     | Responsibility                                              |
|-----------|-------------------------------------------------------------|
| `board`   | Grid state, ship placement, hit/miss logic                  |
| `player`  | Human input and shooting                                    |
| `enemy`   | AI ship placement and AI shooting logic                     |
| `display` | All console output and screen management                    |
| `game`    | Game loop, setup phase, play phase, and main menu           |
