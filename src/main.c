

#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define TILE_SIZE 80  // Taille de chaque case

// Fonction pour convertir les coordonnées de la souris en indices de case
void getMousePosition(int *x, int *y) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    *x = mouseX / TILE_SIZE;
    *y = mouseY / TILE_SIZE;
}


int main() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("Erreur d'initialisation de SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window* window = SDL_CreateWindow("Échecs SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger les textures des pièces
    SDL_Texture* pieces[12];
    pieces[0]  = loadTexture(renderer, "assets/bp.png"); // Pion blanc
    pieces[1]  = loadTexture(renderer, "assets/wp.png"); // Pion noir
    pieces[2]  = loadTexture(renderer, "assets/br.png"); // Tour blanche
    pieces[3]  = loadTexture(renderer, "assets/wr.png"); // Tour noire
    pieces[4]  = loadTexture(renderer, "assets/bn.png"); // Cavalier blanc
    pieces[5]  = loadTexture(renderer, "assets/wn.png"); // Cavalier noir
    pieces[6]  = loadTexture(renderer, "assets/bb.png"); // Fou blanc
    pieces[7]  = loadTexture(renderer, "assets/wb.png"); // Fou noir
    pieces[8]  = loadTexture(renderer, "assets/bq.png"); // Reine blanche
    pieces[9]  = loadTexture(renderer, "assets/wq.png"); // Reine noire
    pieces[10] = loadTexture(renderer, "assets/bk.png"); // Roi blanc
    pieces[11] = loadTexture(renderer, "assets/wk.png"); // Roi noir

    // Vérifier que toutes les textures sont chargées
    for (int i = 0; i < 12; i++) {
        if (!pieces[i]) {
            printf("Erreur: Impossible de charger une texture.\n");
            for (int j = 0; j < 12; j++) {
                if (pieces[j]) SDL_DestroyTexture(pieces[j]);
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return 1;
        }
    }

    // Initialiser le plateau
    char board[8][8];
    initializeBoard(board);

    int gameOver = 0;
    int currentPlayer = 1; // 1 pour blanc, 2 pour noir

    // Variables pour la gestion des clics de la souris
    GamePosition selectedPiece = {-1, -1};  // -1 signifie qu'aucune pièce n'est sélectionnée

    // Gestion des événements SDL
    SDL_Event event;
    while (!gameOver) {
        bool validMove = false;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameOver = 1;
                break;
            }

            // Gestion des clics de la souris
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x, y;
                getMousePosition(&x, &y);

                if (selectedPiece.position.x == -1 && board[y][x] != '.') {
                    // Première sélection : choisir une pièce
                    selectedPiece.position.x = x;
                    selectedPiece.position.y = y;
                } else if (selectedPiece.position.x != -1) {
                    // Deuxième clic : déplacer la pièce
                    GamePosition to = {x, y};
                    movePiece(board, selectedPiece, to, currentPlayer);
                    selectedPiece.position.x = -1; // Réinitialiser la sélection
                    validMove = true;
                }
            }
        }

        // Affichage du plateau et des pièces
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fond blanc
        SDL_RenderClear(renderer);

        // Dessiner l'échiquier et les pièces
        drawBoard(renderer);
        drawPieces(renderer, pieces, board);

        SDL_RenderPresent(renderer);

        // Affichage du plateau sous forme textuelle pour les joueurs
        affichage(board);

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
        if (validMove) {
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }

        // Ajouter un léger délai pour éviter une boucle trop rapide et gourmande en ressources
        SDL_Delay(100); // 100ms
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
