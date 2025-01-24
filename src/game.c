#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "Piece.h"
#include "Position.h"
#include "game.h"

bool roundG(char board[8][8], int currentPlayer)
{
    char input[10];
    printf("Player %d, enter a movement (ex: e2 e4) : ", currentPlayer);
    fgets(input, sizeof(input), stdin);

    GamePosition from = transformer(strtok(input, " "));
    GamePosition to = transformer(strtok(NULL, " "));

    if (isValidMove(board, from, to, currentPlayer)) {
        movePiece(board, from, to, currentPlayer);
        return true;
    } else {
        printf("Try again. Invalid movement\n");
        return false;
    }

}


GamePosition transformer(char *input)
{
    GamePosition pos;

    // 0 is a letter and 1 is a digit
    if (input != NULL && strlen(input) >= 2) {
        pos.position.x = tolower(input[0]) - 'a';
        pos.position.y = 8 - (input[1] - '0');  // Correct conversion of the digit
    } else {
        pos.position.x = -1;
        pos.position.y = -1;
    }

    return pos;
}


bool isPiecePresent(Piece piece, GamePosition pos) {
    if (pos.piece.type != '\0') {
        return piece.color != pos.piece.color;
    }
    return false;
}

bool isValidMove(char board[8][8], GamePosition from, GamePosition to, int currentPlayer) {
    // Check if the positions are valid
    if (!isInBorder(from.position) || !isInBorder(to.position)) {
        return false;
    }

    char piece = board[from.position.y][from.position.x];

    // Check if the starting position contains a piece
    if (piece == '.') {
        return false;
    }

    // Check if the piece belongs to the active player
    bool isWhite = (piece >= 'A' && piece <= 'Z');

    // Create the Piece structure
    Piece p = {
        .type = toupper(piece),
        .color = isWhite // true for white, false for black
    };

    // Check if the move is valid for this piece
    return isValidMovement(board, from.position, to.position, p, currentPlayer);
}


void movePiece(char board[8][8], GamePosition from, GamePosition to, int currentPlayer) {
    
    if (isValidMove(board, from, to, currentPlayer) == false) {
        printf("Invalide movement\n");
        return;
    }
    
    printf("Move : %c from %c%d to %c%d\n", 
           board[from.position.y][from.position.x],
           'a' + from.position.x,
           8 - from.position.y,
           'a' + to.position.x,
           8 - to.position.y);
           
    char piece = board[from.position.y][from.position.x];
    board[from.position.y][from.position.x] = '.';
    board[to.position.y][to.position.x] = piece;
}

