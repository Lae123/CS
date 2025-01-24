#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>
#include "Position.h"
typedef struct {
    char type;  // 'P', 'T', 'C', 'F', 'R', 'D' pour les pièces
    bool color; // true pour blanc, false pour noir
}Piece;


bool isValidMovement(char board[8][8], Position from, Position to, Piece piece);
bool isInBorder(Position pos);
bool movePion(Position from, Position to, Piece piece);
bool moveTour(char board[8][8], Position from, Position to);
bool moveCavalier(Position from, Position to);
bool moveFou(char board[8][8],Position from, Position to);
bool moveReine(char board[8][8], Position from, Position to);
bool moveRoi(char board[8][8], Position from, Position to);
bool isPathClear(char board[8][8], Position from, Position to);

#endif