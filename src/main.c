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
        bool validMove = false;

        while (!validMove) {
            validMove = roundG(board, currentPlayer);
        }

        // Changement de joueur après un mouvement valide
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    return 0;
}
