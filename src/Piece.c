#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

bool isValidMovement(char board[8][8], Position from, Position to, Piece piece){
    bool res = true;
    switch (piece.type) {
        case 'P': // Pion
            res = movePion(board, from, to, piece);
            break;
        case 'R': // Tour (Rook)
            res = moveTour(board, from, to);
            break;
        case 'N': // Cavalier (kNight)
            res = moveCavalier(board, from, to);
            break;
        case 'B': // Fou (Bishop)
            res = moveFou(board, from, to);
            break;
        case 'K': // Roi (King)
            res = moveRoi(board, from, to);
            break;
        case 'Q': // Dame (Queen)
            res = moveReine(board, from, to);
            break;
    }
    return res;
}


// Vérifie si une position est dans les limites de l'échiquier
bool isInBorder(Position pos) {
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

// Vérifie les mouvements du pion
bool movePion(char board[8][8], Position from, Position to, Piece piece) {
    bool res = true;

    // Vérifier si la destination est dans les limites
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    int direction = piece.color ? 1 : -1; // Blanc avance (+1), noir (-1)

    // Mouvements verticaux (1 ou 2 cases au début)
    if (from.x == to.x) {
        if ((to.y == from.y + direction) || 
            (from.y == (piece.color ? 1 : 6) && to.y == from.y + 2 * direction)) {
            return res;
        }
    }

    // Capture diagonale
    if (abs(to.x - from.x) == 1 && to.y == from.y + direction) {
        return res;
    }

    return false;
}

// Vérifie les mouvements de la tour
bool moveTour(char board[8][8], Position from, Position to) {
    bool res = true;

    // Vérifier si la destination est dans les limites
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    // Mouvement horizontal ou vertical
    if (from.x == to.x || from.y == to.y) {
        return res;
    }

    return false;
}

// Vérifie les mouvements du cavalier
bool moveCavalier(char board[8][8], Position from, Position to) {
    // Différences possibles pour un mouvement en L
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

    // Vérifier si la destination est dans les limites
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    for (int i = 0; i < 8; i++) {
        if (from.x + dx[i] == to.x && from.y + dy[i] == to.y) {
            return true;
        }
    }

    return false;
}

// Vérifie les mouvements du fou
bool moveFou(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    // Mouvement en diagonale
    if (abs(to.x - from.x) == abs(to.y - from.y)) {
        return true;
    }

    return false;
}

// Vérifie les mouvements de la reine
bool moveReine(char board[8][8], Position from, Position to) {
    return moveTour(board, from, to) || moveFou(board, from, to);
}

// Vérifie les mouvements du roi
bool moveRoi(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    // Une case dans n'importe quelle direction
    if (abs(to.x - from.x) <= 1 && abs(to.y - from.y) <= 1) {
        return true;
    }

    return false;
}
