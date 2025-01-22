#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "game.h"

int main() {
    char board[8][8];
    initializeBoard(board);
    int gameOver = 0;
    int currentPlayer = 1;
    
    while (!gameOver) {
        affichage(board);
        bool validMove = 0;
        while (!validMove) {
            validMove = roundG(board, currentPlayer);
            if (!validMove) {
                printf("Mouvement invalide. Réessayez.\n");
            }
        }
    }
    return 0;
}