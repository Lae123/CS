#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

// Vérifie si une position est dans les limites de l'échiquier
bool isInBorder(Position pos) {
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

// Vérifie si le chemin entre deux positions est dégagé
bool isPathClear(char board[8][8], Position from, Position to) {
    int dx = (to.x > from.x) ? 1 : (to.x < from.x) ? -1 : 0;
    int dy = (to.y > from.y) ? 1 : (to.y < from.y) ? -1 : 0;

    int x = from.x + dx;
    int y = from.y + dy;

    while (x != to.x || y != to.y) {
        if (board[y][x] != '.') {
            return false;
        }
        x += dx;
        y += dy;
    }

    return true;
}

// Vérifie les mouvements du pion
bool movePion(Position from, Position to, Piece piece) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    int direction = piece.color ? 1 : -1; // Blanc avance (+1), noir (-1)

    // Mouvements verticaux
    if (from.x == to.x) {
        if (to.y == from.y + direction) {
            return true;
        }
        // Deux cases au premier mouvement
        if (from.y == (piece.color ? 1 : 6) && to.y == from.y + 2 * direction) {
            return true;
        }
    }

    // Capture diagonale
    if (abs(to.x - from.x) == 1 && to.y == from.y + direction) {
        return true;
    }

    return false;
}

// Vérifie les mouvements de la tour
bool moveTour(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    if (from.x == to.x || from.y == to.y) {
        return isPathClear(board, from, to);
    }

    return false;
}

// Vérifie les mouvements du cavalier
bool moveCavalier(Position from, Position to) {
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

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

    if (abs(to.x - from.x) == abs(to.y - from.y)) {
        return isPathClear(board, from, to);
    }

    return false;
}

// Vérifie les mouvements de la reine
bool moveReine(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    return moveTour(board, from, to) || moveFou(board, from, to);
}

// Vérifie les mouvements du roi
bool moveRoi(char board[8][8], Position from, Position to) {
    if (!isInBorder(from) || !isInBorder(to)) {
        return false;
    }

    if (abs(to.x - from.x) <= 1 && abs(to.y - from.y) <= 1) {
        return true;
    }

    return false;
}

// Vérifie si un mouvement est valide pour une pièce donnée
bool isValidMovement(char board[8][8], Position from, Position to, Piece piece) {
    switch (piece.type) {
        case 'P': // Pion
            return movePion(from, to, piece);
        case 'R': // Tour (Rook)
            return moveTour(board, from, to);
        case 'N': // Cavalier (Knight)
            return moveCavalier(from, to);
        case 'B': // Fou (Bishop)
            return moveFou(board, from, to);
        case 'Q': // Dame (Queen)
            return moveReine(board, from, to);
        case 'K': // Roi (King)
            return moveRoi(board, from, to);
        default:
            return false;
    }
}
