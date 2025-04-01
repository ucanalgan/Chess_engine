# Chess Engine

## Summary
A terminal-based chess game implementation in C programming language that provides a complete chess experience with move validation, timing, and game state management. The project demonstrates advanced programming concepts including data structures, algorithm implementation, and user interaction handling.

## Description
This chess engine is designed to provide a comprehensive chess playing experience in a terminal environment. It features a clean, intuitive interface with colored piece representation and real-time game state tracking. The implementation focuses on both functionality and user experience, making it suitable for both learning and recreational purposes.

### Key Components
1. **Game Engine**
   - 8x8 chess board implementation
   - Complete piece movement validation
   - Turn-based gameplay system
   - Check and checkmate detection
   - Stalemate detection

2. **User Interface**
   - Colored console output for better visibility
   - Clear board representation
   - Intuitive move input system
   - Real-time game status display

3. **Game Features**
   - Move timing for each player
   - Total game time tracking
   - Move counter
   - Piece capture animation
   - Game save/load functionality

4. **Technical Implementation**
   - Structured data management using C structs
   - Efficient board representation
   - Modular code design
   - Windows console integration for enhanced display

### Project Structure
```
chess_engine/
├── main.c          # Core game logic and implementation
├── README.md       # Project documentation
└── *.sav           # Saved game files
```

### Core Features
- 8x8 chess board simulation
- Correct initial piece placement
- Turn-based gameplay (White/Black)
- Move validation for all pieces
- Legal move checking
- Check and checkmate detection
- Stalemate detection

### Additional Features
- Move timer for each player
- Total game time tracking
- Move counter
- Piece capture animation
- Colored console output
- Game save/load functionality
- Castling support (coming soon)
- Pawn promotion (coming soon)

### Technical Details

#### Data Structures
- 2D character array for the board
- GameState struct for managing game state
- Time tracking for each player

#### Piece Representation
- Uppercase letters: White pieces
- Lowercase letters: Black pieces
- P/p: Pawn
- R/r: Rook
- N/n: Knight
- B/b: Bishop
- Q/q: Queen
- K/k: King
- .: Empty square

## Building and Running

### Prerequisites
- GCC compiler
- Windows operating system (for colored output)

### Compilation
```powershell
gcc main.c -o chess
```

### Running the Game
```powershell
.\chess
```

## Game Commands

### Move Input
Moves should be entered in "e2 e4" format:
- First character: Starting column (a-h)
- Second character: Starting row (1-8)
- Space
- Target column (a-h)
- Target row (1-8)

### Special Commands
- `save <filename>`: Save current game state
- `load <filename>`: Load a saved game
- `quit`: Exit the game

## Game Rules

### Basic Rules
1. White moves first
2. Players alternate turns
3. Pieces move according to standard chess rules
4. Captured pieces are removed from the board

### Special Rules
- Check: When a king is under attack
- Checkmate: When a king has no legal moves and is under attack
- Stalemate: When a player has no legal moves but is not in check

## Future Improvements
1. Implement complete piece movement rules
2. Add castling functionality
3. Add pawn promotion
4. Implement move history
5. Add AI opponent
6. Add network multiplayer support
7. Improve capture animation
8. Add move sound effects

## Contributing
Feel free to submit issues and enhancement requests!

## License
This project is licensed under the MIT License - see the LICENSE file for details. 
