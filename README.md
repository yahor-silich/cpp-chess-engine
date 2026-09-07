# C++ Chess Engine

A clean, object-oriented/procedural console-based chess engine implemented from scratch in C++. Developed as a portfolio project to demonstrate strong programming fundamentals, algorithm design, and software engineering practices.

## Features

- **Complete Move Validation**: Full rules implementation for all chess pieces (Pawns, Rooks, Knights, Bishops, Queens, Kings).
- **Check Detection**: Real-time evaluation of board states to prevent illegal moves that leave the king under attack.
- **Special Moves**: 
  - Pawn promotion (automatic conversion to Queen upon reaching the final rank).
  - Castling (fully validated for both kingside and queenside, accounting for path clearance and square safety).
- **Console Interface**: Interactive command-line loop utilizing algebraic notation (e.g., `e2e4`) with visual board rendering.

## Tech Stack

- **Language**: C++
- **Version Control**: Git / GitHub
- **Environment**: VS Code (macOS)

## Getting Started

### Prerequisites
You need a C++ compiler installed on your system (e.g., Clang, GCC, or MSVC).

### Compilation & Running
Clone the repository and compile the source file using your terminal:

```bash
git clone https://github.com/yahor-silich/cpp-chess-engine.git
cd cpp-chess-engine
g++ chess.cpp -o chess
./chess 
```