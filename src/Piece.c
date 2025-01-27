#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

// Checks if a position is within the boundaries of the chessboard
bool isInBorder(Position pos) {
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

// Checks if the path between two positions is clear
bool isPathClear(char board[8][8], Position from, Position to) {
    int dx = (to.x > from.x) ? 1 : (to.x < from.x) ? -1 : 0;
    int dy = (to.y > from.y) ? 1 : (to.y < from.y) ? -1 : 0;

    int x = from.x + dx;
    int y = from.y + dy;

    while (x != to.x || y != to.y) {
        if (board[y][x] != '.') {
            return false;
        }
        x += dx;
        y += dy;
    }

    return true;
}

// Checks the movements of the pawn
bool movePion(Position from, Position to, Piece piece, int curr) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    int direction =0;
    if(curr == 2){
        direction = 1;
    }else{
        direction = -1;
    }

    // Vertical movements
    if (from.x == to.x) {
        if (to.y == from.y + direction) {
            return true;
        }
        // Two squares on the first move
        if (from.y == (piece.color ? 1 : 6) && to.y == from.y + 2 * direction) {
            return true;
        }
    }

    // Diagonal capture
    if (abs(to.x - from.x) == 1 && to.y == from.y + direction) {
        return true;
    }

    return false;
}

// Checks the movements of the rook
bool moveTour(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    if (from.x == to.x || from.y == to.y) {
        return isPathClear(board, from, to);
    }

    return false;
}

// Checks the movements of the knight
bool moveCavalier(Position from, Position to) {
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    for (int i = 0; i < 8; i++) {
        if (from.x + dx[i] == to.x && from.y + dy[i] == to.y) {
            return true;
        }
    }

    return false;
}

// Checks the movements of the bishop
bool moveFou(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    if (abs(to.x - from.x) == abs(to.y - from.y)) {
        return isPathClear(board, from, to);
    }

    return false;
}

// Checks the movements of the queen
bool moveReine(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    return moveTour(board, from, to) || moveFou(board, from, to);
}

// Checks the movements of the king
bool moveRoi(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    if (abs(to.x - from.x) <= 1 && abs(to.y - from.y) <= 1) {
        return isPathClear(board, from, to) && !isCheckmate(board, board[from.y][from.x] >= 'A' && board[from.y][from.x] <= 'Z');
    }

    return false;
}

// Checks if a movement is valid for a given piece
bool isValidMovement(char board[8][8], Position from, Position to, Piece piece, int curr) {
    switch (piece.type) {
        case 'P': // Pawn
            return movePion(from, to, piece, curr);
        case 'R': // (Rook)
            return moveTour(board, from, to);
        case 'N': // (Knight)
            return moveCavalier(from, to);
        case 'B': // (Bishop)
            return moveFou(board, from, to);
        case 'Q': // (Queen)
            return moveReine(board, from, to);
        case 'K': // (King)
            return moveRoi(board, from, to);
        default:
            return false;
    }
}

// Function to detect if a king is in check
// bool isCheck(char board[8][8], Position kingPos, bool isWhite) {
//     Position from;
//     for (int y = 0; y < 8; y++) {
//         for (int x = 0; x < 8; x++) {
//             char piece = board[y][x];

//             // Check opponent's pieces
//             if ((isWhite && piece >= 'a' && piece <= 'z') || (!isWhite && piece >= 'A' && piece <= 'Z')) {
//                 from.x = x;
//                 from.y = y;

//                 Piece p = { .type = piece, .color = !isWhite };
//                 if (isValidMovement(board, from, kingPos, p, !isWhite ? 1 : 2)) {
//                     printf("Check detected: %c at (%d, %d) can move to king at (%d, %d)\n", piece, x, y, kingPos.x, kingPos.y);
//                     return true;
//                 }
//             }
//         }
//     }
//     printf("No check detected for king at (%d, %d)\n", kingPos.x, kingPos.y);
//     return false;
// }

bool isCheck(char board[8][8], Position kingPos, bool isWhite) {
    Position from;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char piece = board[y][x];
            
            if ((isWhite && piece >= 'a' && piece <= 'z') || 
                (!isWhite && piece >= 'A' && piece <= 'Z')) {
                
                from.x = x;
                from.y = y;

                Piece p = { 
                    .type = toupper(piece),
                    .color = !isWhite  
                };

                int curr = (piece >= 'A' && piece <= 'Z') ? 1 : 2;
                
                if (isValidMovement(board, from, kingPos, p, curr)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Checks if a player is in checkmate
bool isCheckmate(char board[8][8], bool isWhite) {
    Position kingPos = findKingPos(board, isWhite);

    if (!isCheck(board, kingPos, isWhite)) {
        return false;
    }

    // Look if the king can escape
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            // Ignore the case where the king doesn't move
            if (dx == 0 && dy == 0) continue;

            Position newPos = { .x = kingPos.x + dx, .y = kingPos.y + dy };

            if (!isInBorder(newPos)) continue;

            // Look if the case is occupied by a friendly piece
            char target = board[newPos.y][newPos.x];
            if ((isWhite && target >= 'A' && target <= 'Z') || 
                (!isWhite && target >= 'a' && target <= 'z')) {
                continue;
            }

            // Simulate the movement of the king
            char tempBoard[8][8];
            memcpy(tempBoard, board, sizeof(tempBoard));
            tempBoard[newPos.y][newPos.x] = board[kingPos.y][kingPos.x];
            tempBoard[kingPos.y][kingPos.x] = '.';

            // Look if the king can escape
            if (!isCheck(tempBoard, newPos, isWhite)) {
                return false; // The king can escape
            }
        }
    }

    // If no legal move exists to escape the check
    return true;
}



Position findKingPos(char board[8][8], bool isWhite) {
    Position kingPos;
    char king = isWhite ? 'K' : 'k';

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board[y][x] == king) {
                kingPos.x = x;
                kingPos.y = y;
                return kingPos;
            }
        }
    }

    // Return an invalid position if the king is not found
    kingPos.x = -1;
    kingPos.y = -1;
    return kingPos;
}
