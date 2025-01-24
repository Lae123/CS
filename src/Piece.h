#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>
#include "Position.h"
typedef struct {
    char type;  // 'P', 'T', 'C', 'F', 'R', 'D' for the pieces
    bool color; // true for white, false for black
}Piece;


bool isValidMovement(char board[8][8], Position from, Position to, Piece piece, int curr);
bool isInBorder(Position pos);
bool movePion(Position from, Position to, Piece piece, int curr) ;
bool moveTour(char board[8][8], Position from, Position to);
bool moveCavalier(Position from, Position to);
bool moveFou(char board[8][8],Position from, Position to);
bool moveReine(char board[8][8], Position from, Position to);
bool moveRoi(char board[8][8], Position from, Position to);
bool isPathClear(char board[8][8], Position from, Position to);

bool isCheckmate(char board[8][8], bool isWhite);
bool isCheck(char board[8][8], Position kingPos, bool isWhite);
Position findKingPos(char board[8][8], bool isWhite);

#endif