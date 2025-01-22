#include <stdio.h>
#include "board.h"
#include "game.h"

int main (){
    char board[8][8];
    initializeBoard(board);
    affichage(board);

    roundG(board, 1);

    return 0;
}