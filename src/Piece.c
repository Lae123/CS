#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
bool isCheck(char board[8][8], Position kingPos, bool isWhite) {
    Position from;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char piece = board[y][x];

            // Check opponent's pieces
            if ((isWhite && piece >= 'a' && piece <= 'z') || (!isWhite && piece >= 'A' && piece <= 'Z')) {
                from.x = x;
                from.y = y;

                Piece p = { .type = piece, .color = !isWhite };
                if (isValidMovement(board, from, kingPos, p, isWhite ? 1 : 2)) {
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

    // If the king is not in check, it is not checkmate
    if (!isCheck(board, kingPos, isWhite)) {
        return false;
    }

    // Check all possible moves of the player
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char piece = board[y][x];

            // Check only the player's pieces
            if ((isWhite && piece >= 'A' && piece <= 'Z') || (!isWhite && piece >= 'a' && piece <= 'z')) {
                Position from = { .x = x, .y = y };

                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;

                        Position to = { .x = x + dx, .y = y + dy };

                        if (isInBorder(to) && isValidMovement(board, from, to, (Piece){.type = piece, .color = isWhite}, isWhite ? 1 : 2)) {
                            // Copy the board to simulate the move
                            char tempBoard[8][8];
                            memcpy(tempBoard, board, sizeof(tempBoard));

                            // Simulate the move
                            tempBoard[to.y][to.x] = tempBoard[from.y][from.x];
                            tempBoard[from.y][from.x] = '.';

                            // Check if this is still a check 
                            Position newKingPos = findKingPos(tempBoard, isWhite);
                            if (!isCheck(tempBoard, newKingPos, isWhite)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    // No legal move to get out of check -> checkmate
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
