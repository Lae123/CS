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
    printf("Joueur %d, entrez votre mouvement (ex: e2 e4) : ", currentPlayer);
    fgets(input, sizeof(input), stdin);

    GamePosition from = transformer(strtok(input, " "));
    GamePosition to = transformer(strtok(NULL, " "));

    printf("Debug: from position - x: %d, y: %d\n", from.position.x, from.position.y);
    printf("Debug: to position - x: %d, y: %d\n", to.position.x, to.position.y);

    if (isValidMove(board, from, to, currentPlayer)) {
        movePiece(board, from, to, currentPlayer);
        return true;
    } else {
        printf("Mouvement invalide. Réessayez.\n");
        return false;
    }

}


GamePosition transformer(char *input)
{
    GamePosition pos;

    // 0 c'est une lettre et 1 c'est un chiffre
    if (input != NULL && strlen(input) >= 2) {
        pos.position.x = tolower(input[0]) - 'a';
        pos.position.y = 8 - (input[1] - '0');  // Conversion correcte du chiffre
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
    // Vérifier si les positions sont valides
    if (!isInBorder(from.position) || !isInBorder(to.position)) {
        printf("oaaaa c moiiii");
        return false;
    }

    char piece = board[from.position.y][from.position.x];

    // Vérifier si la position de départ contient une pièce
    if (piece == '.') {
        printf("oiu c mooii");
        return false;
    }

    // Vérifier si la pièce appartient au joueur actif
    bool isWhite = (piece >= 'A' && piece <= 'Z');
    // if ((currentPlayer == 1 && !isWhite) || (currentPlayer == 2 && isWhite)) {
    //     printf("ouiii ci moi");
    //     return false;  // Pièce appartenant à l'adversaire
    // }

    // Créer la structure Piece
    Piece p = {
        .type = toupper(piece),
        .color = isWhite // true pour blanc, false pour noir
    };

    // Vérifier si le mouvement est valide pour cette pièce
    return isValidMovement(board, from.position, to.position, p, currentPlayer);
}


void movePiece(char board[8][8], GamePosition from, GamePosition to, int currentPlayer) {
    
    if (isValidMove(board, from, to, currentPlayer) == false) {
        printf("Mouvement invalide\n");
        return;
    }
    
    printf("Déplacement: %c de %c%d vers %c%d\n", 
           board[from.position.y][from.position.x],
           'a' + from.position.x,
           8 - from.position.y,
           'a' + to.position.x,
           8 - to.position.y);
           
    char piece = board[from.position.y][from.position.x];
    board[from.position.y][from.position.x] = '.';
    board[to.position.y][to.position.x] = piece;
}