#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "Piece.h"
#include "Position.h"
#include "game.h"

void roundG(char board[8][8], int currentPlayer)
{
    char input[10];
    printf("Joueur %d, entrez votre mouvement (ex: e2 e4) : ", currentPlayer);
    fgets(input, sizeof(input), stdin);

    GamePosition from = transformer(strtok(input, " "));
    GamePosition to = transformer(strtok(NULL, " "));

    if (isValidMove(board, from, to)) {
        movePiece(board, from, to);
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    } else {
        printf("Mouvement invalide. Réessayez.\n");
    }
}

GamePosition transformer(char *input)
{
    GamePosition pos;

    // 0 c'est une lettre et 1 c'est un chiffre
    if(input != NULL && strlen(input) == 2 && isalpha(input[0]) && isdigit(input[1])){
        pos.position.x = input[0] - 'a';
        pos.position.y = 8 - (input[1] - '0');
    }else{
        pos.position.x = -1;
        pos.position.y = -1;
    }

    return pos;
}

bool isValidMove(char board[8][8], GamePosition from, GamePosition to)
{

    if(((from.position.x > 8 || from.position.x < 0) && (from.position.y > 8 || from.position.y < 0)) || ((to.position.x > 8 || to.position.x < 0) && (to.position.y > 8 || to.position.y < 0))){
        return false;
    }


    return isValidMovement(board, from.position, to.position, from.piece);
}

void movePiece(char board[8][8], GamePosition from, GamePosition to)
{ 
    if(isValidMove(board, from, to) == 1){
        printf("The movement is invalid, you cannot move the piece that way");
    }else{
        Piece tab = from.piece;
        from.piece = (Piece){0}; // Valeur par default
        to.piece = tab;
    }
}