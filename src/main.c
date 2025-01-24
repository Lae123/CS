#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "game.h"

int main() {
    char board[8][8];
    initializeBoard(board); // Initialise l'échiquier
    int gameOver = 0;
    int currentPlayer = 1; // 1 pour les blancs, 2 pour les noirs

    while (!gameOver) {
        affichage(board); // Affiche l'échiquier

        bool validMove = false;

        while (!validMove) {
            validMove = roundG(board, currentPlayer); // Le joueur effectue un mouvement
        }

        // Vérification de l'échec ou de l'échec et mat
        bool isWhite = (currentPlayer == 1);
        if (isCheck(board, findKingPos(board, isWhite), isWhite)) {
            printf("Le roi du joueur %d est en échec !\n", currentPlayer);

            if (isCheckmate(board, isWhite)) {
                affichage(board);
                printf("Échec et mat ! Le joueur %d a perdu.\n", currentPlayer);
                gameOver = 1;
                break;
            }
        }

        // Changement de joueur après un mouvement valide
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    printf("Partie terminée ! Merci d'avoir joué.\n");
    return 0;
}
