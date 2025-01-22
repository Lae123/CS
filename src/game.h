#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include "Position.h"
#include "Piece.h"

typedef struct {
    Position position;
    Piece piece;
} GamePosition;

bool roundG(char board[8][8], int currentPlayer);
GamePosition transformer(char* input);
bool isValidMove(char board[8][8], GamePosition from, GamePosition to);
void movePiece(char board[8][8], GamePosition from, GamePosition to);

#endif