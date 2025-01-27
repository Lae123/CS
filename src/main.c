#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "game.h"

int main() {
    char board[8][8];
    initializeBoard(board); // Initialize the chessboard
    int gameOver = 0;
    int currentPlayer = 1; // 1 for white, 2 for black

    while (!gameOver) {
        affichage(board); // Display the chessboard

        bool validMove = false;

        while (!validMove) {
            validMove = roundG(board, currentPlayer); // The player makes a move
        }

        // Check for check or checkmate
        bool isWhite = (currentPlayer == 1);
        Position kingPos = findKingPos(board, isWhite);
        
        if (isCheck(board, kingPos, isWhite)) {
            printf("Your king is in danger!\n");

            if (isCheckmate(board, isWhite)) {
                affichage(board);
                printf("Checkmate! Player %d won.\n", currentPlayer);
                gameOver = 1;
                break;
            }
        }

        // Change player after a valid move
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    printf("Good game! Thank you for playing.\n");
    return 0;
}
