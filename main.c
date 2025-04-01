#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

// Chess piece characters
#define EMPTY '.'
#define WHITE_PAWN 'P'
#define WHITE_ROOK 'R'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_QUEEN 'Q'
#define WHITE_KING 'K'
#define BLACK_PAWN 'p'
#define BLACK_ROOK 'r'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_QUEEN 'q'
#define BLACK_KING 'k'

// Game state
struct GameState {
    char board[8][8];
    int currentPlayer;  // 1: White, 0: Black
    int moveCount;
    time_t startTime;
    time_t whiteTime;
    time_t blackTime;
    int whiteKingMoved;
    int blackKingMoved;
    int whiteRooksMoved[2];
    int blackRooksMoved[2];
};

// Function prototypes
void initializeBoard(struct GameState* game);
void printBoard(const struct GameState* game);
int isValidMove(const struct GameState* game, int fromRow, int fromCol, int toRow, int toCol);
void makeMove(struct GameState* game, int fromRow, int fromCol, int toRow, int toCol);
int parseMove(const char* move, int* fromRow, int* fromCol, int* toRow, int* toCol);
void animateCapture(int fromRow, int fromCol, int toRow, int toCol);
int isCheck(const struct GameState* game, int isWhite);
int isCheckmate(const struct GameState* game, int isWhite);
int isStalemate(const struct GameState* game, int isWhite);
void saveGame(const struct GameState* game, const char* filename);
int loadGame(struct GameState* game, const char* filename);
void printTimer(const struct GameState* game);
void updateTimer(struct GameState* game);

// Helper functions for colored console output
void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printColoredPiece(char piece) {
    if (isupper(piece)) {
        setConsoleColor(15); // White
    } else {
        setConsoleColor(8);  // Gray (Black)
    }
    printf("%c ", piece);
    setConsoleColor(7); // Normal color
}

int main() {
    struct GameState game;
    char move[10];
    int fromRow, fromCol, toRow, toCol;
    char filename[256];
    
    // Initialize game
    initializeBoard(&game);
    game.startTime = time(NULL);
    game.whiteTime = 0;
    game.blackTime = 0;
    game.moveCount = 0;
    game.whiteKingMoved = 0;
    game.blackKingMoved = 0;
    memset(game.whiteRooksMoved, 0, sizeof(game.whiteRooksMoved));
    memset(game.blackRooksMoved, 0, sizeof(game.blackRooksMoved));
    
    while (1) {
        system("cls"); // Clear screen
        printBoard(&game);
        printTimer(&game);
        
        printf("\n%s player's turn (example: e2 e4)\n", game.currentPlayer ? "White" : "Black");
        printf("Commands: save <file> - load <file> - quit\n");
        printf("Move: ");
        fgets(move, sizeof(move), stdin);
        
        // Command check
        if (strncmp(move, "save ", 5) == 0) {
            sscanf(move + 5, "%s", filename);
            saveGame(&game, filename);
            printf("Game saved: %s\n", filename);
            continue;
        }
        else if (strncmp(move, "load ", 5) == 0) {
            sscanf(move + 5, "%s", filename);
            if (loadGame(&game, filename)) {
                printf("Game loaded: %s\n", filename);
            } else {
                printf("Failed to load game!\n");
            }
            continue;
        }
        else if (strncmp(move, "quit", 4) == 0) {
            break;
        }
        
        if (parseMove(move, &fromRow, &fromCol, &toRow, &toCol)) {
            if (isValidMove(&game, fromRow, fromCol, toRow, toCol)) {
                // Capture animation
                if (game.board[toRow][toCol] != EMPTY) {
                    animateCapture(fromRow, fromCol, toRow, toCol);
                }
                
                makeMove(&game, fromRow, fromCol, toRow, toCol);
                game.moveCount++;
                game.currentPlayer = !game.currentPlayer;
                
                // Checkmate / Stalemate check
                if (isCheckmate(&game, !game.currentPlayer)) {
                    printBoard(&game);
                    printf("\n%s player wins! Checkmate!\n", game.currentPlayer ? "Black" : "White");
                    break;
                }
                if (isStalemate(&game, !game.currentPlayer)) {
                    printBoard(&game);
                    printf("\nGame draw! Stalemate!\n");
                    break;
                }
            } else {
                printf("Invalid move!\n");
            }
        } else {
            printf("Invalid input! Please enter in 'e2 e4' format.\n");
        }
    }
    
    return 0;
}

void initializeBoard(struct GameState* game) {
    // Initialize empty board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            game->board[i][j] = EMPTY;
        }
    }
    
    // Place white pieces
    game->board[7][0] = game->board[7][7] = WHITE_ROOK;
    game->board[7][1] = game->board[7][6] = WHITE_KNIGHT;
    game->board[7][2] = game->board[7][5] = WHITE_BISHOP;
    game->board[7][3] = WHITE_QUEEN;
    game->board[7][4] = WHITE_KING;
    for (int i = 0; i < 8; i++) {
        game->board[6][i] = WHITE_PAWN;
    }
    
    // Place black pieces
    game->board[0][0] = game->board[0][7] = BLACK_ROOK;
    game->board[0][1] = game->board[0][6] = BLACK_KNIGHT;
    game->board[0][2] = game->board[0][5] = BLACK_BISHOP;
    game->board[0][3] = BLACK_QUEEN;
    game->board[0][4] = BLACK_KING;
    for (int i = 0; i < 8; i++) {
        game->board[1][i] = BLACK_PAWN;
    }
}

void printBoard(const struct GameState* game) {
    printf("\n  a b c d e f g h\n");
    printf("  ---------------\n");
    for (int i = 0; i < 8; i++) {
        printf("%d|", 8 - i);
        for (int j = 0; j < 8; j++) {
            printColoredPiece(game->board[i][j]);
        }
        printf("|%d\n", 8 - i);
    }
    printf("  ---------------\n");
    printf("  a b c d e f g h\n");
}

void animateCapture(int fromRow, int fromCol, int toRow, int toCol) {
    printf("\nPiece capture animation...\n");
    Sleep(500); // Wait 500ms
}

void printTimer(const struct GameState* game) {
    time_t currentTime = time(NULL);
    time_t elapsed = currentTime - game->startTime;
    
    printf("\nMove count: %d\n", game->moveCount);
    printf("White time: %02d:%02d\n", (int)(game->whiteTime / 60), (int)(game->whiteTime % 60));
    printf("Black time: %02d:%02d\n", (int)(game->blackTime / 60), (int)(game->blackTime % 60));
    printf("Total time: %02d:%02d\n", (int)(elapsed / 60), (int)(elapsed % 60));
}

void updateTimer(struct GameState* game) {
    time_t currentTime = time(NULL);
    static time_t lastUpdate = 0;
    
    if (lastUpdate == 0) {
        lastUpdate = currentTime;
        return;
    }
    
    time_t delta = currentTime - lastUpdate;
    if (game->currentPlayer) {
        game->whiteTime += delta;
    } else {
        game->blackTime += delta;
    }
    lastUpdate = currentTime;
}

int isCheck(const struct GameState* game, int isWhite) {
    // Find king's position
    int kingRow = -1, kingCol = -1;
    char king = isWhite ? WHITE_KING : BLACK_KING;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game->board[i][j] == king) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }
    
    // Check if opponent pieces threaten the king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char piece = game->board[i][j];
            if (isWhite ? islower(piece) : isupper(piece)) {
                // TODO: Add threat check for each piece type
                // For now, we do a simple check
                if (abs(i - kingRow) <= 1 && abs(j - kingCol) <= 1) {
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

int isCheckmate(const struct GameState* game, int isWhite) {
    if (!isCheck(game, isWhite)) return 0;
    
    // Check if king has any legal moves
    // TODO: Check all possible moves
    return 1; // Simple check for now
}

int isStalemate(const struct GameState* game, int isWhite) {
    if (isCheck(game, isWhite)) return 0;
    
    // Check if there are any legal moves
    // TODO: Check all possible moves
    return 1; // Simple check for now
}

void saveGame(const struct GameState* game, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file) {
        fwrite(game, sizeof(struct GameState), 1, file);
        fclose(file);
    }
}

int loadGame(struct GameState* game, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file) {
        fread(game, sizeof(struct GameState), 1, file);
        fclose(file);
        return 1;
    }
    return 0;
}

int parseMove(const char* move, int* fromRow, int* fromCol, int* toRow, int* toCol) {
    if (strlen(move) < 5) return 0;
    
    *fromCol = tolower(move[0]) - 'a';
    *fromRow = 8 - (move[1] - '0');
    *toCol = tolower(move[3]) - 'a';
    *toRow = 8 - (move[4] - '0');
    
    return (*fromRow >= 0 && *fromRow < 8 && *fromCol >= 0 && *fromCol < 8 &&
            *toRow >= 0 && *toRow < 8 && *toCol >= 0 && *toCol < 8);
}

int isValidMove(const struct GameState* game, int fromRow, int fromCol, int toRow, int toCol) {
    char piece = game->board[fromRow][fromCol];
    
    // Empty square check
    if (piece == EMPTY) return 0;
    
    // Piece color check
    int isWhite = isupper(piece);
    if (isWhite != game->currentPlayer) return 0;
    
    // Same color piece in target square check
    if (isWhite == isupper(game->board[toRow][toCol]) && game->board[toRow][toCol] != EMPTY) return 0;
    
    // TODO: Add special movement rules for each piece type
    
    return 1;
}

void makeMove(struct GameState* game, int fromRow, int fromCol, int toRow, int toCol) {
    game->board[toRow][toCol] = game->board[fromRow][fromCol];
    game->board[fromRow][fromCol] = EMPTY;
} 