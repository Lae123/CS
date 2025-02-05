#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define TILE_SIZE 80  // Taille de chaque case

int main() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("Erreur d'initialisation de SDL_image: %s\n", IMG_GetError());
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window* window = SDL_CreateWindow("Échecs SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        return 1;
    }

    // Charger les textures des pièces (fichiers image)
    SDL_Texture* pieces[12];
    pieces[0]  = loadTexture(renderer, "assets/wp.png"); // Pion blanc
    pieces[1]  = loadTexture(renderer, "assets/bp.png"); // Pion noir
    pieces[2]  = loadTexture(renderer, "assets/wr.png"); // Tour blanche
    pieces[3]  = loadTexture(renderer, "assets/br.png"); // Tour noire
    pieces[4]  = loadTexture(renderer, "assets/wn.png"); // Cavalier blanc
    pieces[5]  = loadTexture(renderer, "assets/bn.png"); // Cavalier noir
    pieces[6]  = loadTexture(renderer, "assets/wb.png"); // Fou blanc
    pieces[7]  = loadTexture(renderer, "assets/bb.png"); // Fou noir
    pieces[8]  = loadTexture(renderer, "assets/wq.png"); // Reine blanche
    pieces[9]  = loadTexture(renderer, "assets/bq.png"); // Reine noire
    pieces[10] = loadTexture(renderer, "assets/wk.png"); // Roi blanc
    pieces[11] = loadTexture(renderer, "assets/bk.png"); // Roi noir

    // Initialiser le plateau
    char board[8][8];
    initializeBoard(board);

    int gameOver = 0;
    int currentPlayer = 1; // 1 pour blanc, 2 pour noir

    while (!gameOver) {
        // Affichage du plateau et des pièces
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner l'échiquier
        drawBoard(renderer);

        // Dessiner les pièces sur l'échiquier
        drawPieces(renderer, pieces);

        SDL_RenderPresent(renderer);

        // Affichage du plateau sous forme textuelle pour les joueurs
        affichage(board);

        // Boucle de jeu, faire un mouvement
        bool validMove = false;
        while (!validMove) {
            validMove = roundG(board, currentPlayer); // La fonction de mouvement
        }

        // Vérification du check/checkmate
        bool isWhite = (currentPlayer == 1);
        Position kingPos = findKingPos(board, isWhite);

        if (isCheck(board, kingPos, isWhite)) {
            printf("Votre roi est en danger !\n");

            if (isCheckmate(board, isWhite)) {
                affichage(board);
                printf("Échec et mat ! Le joueur %d a gagné.\n", currentPlayer);
                gameOver = 1;
                break;
            }
        }

        // Changer de joueur après un mouvement valide
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    printf("Bonne partie ! Merci d'avoir joué.\n");

    // Libérer les ressources SDL
    for (int i = 0; i < 12; i++) {
        SDL_DestroyTexture(pieces[i]);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
