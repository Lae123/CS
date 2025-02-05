#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define TILE_SIZE 80  // Chaque case fait 80 pixels

// Échiquier initial
char board[8][8] = {
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
};

void initializeBoard (char board[8][8])
{
    
    char set[8][8] =
    {
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
    };

    for(int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[i][j] = set[i][j];
        }
    }
}


void affichage(char board[8][8]) {
    printf("  a b c d e f g h\n");
    printf("  ----------------\n");
    for (int i = 0; i < 8; i++) {
        printf("%d| ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", board[i][j]);
        }
        printf("|%d\n", 8 - i);
    }
    printf("  ----------------\n");
    printf("  a b c d e f g h\n\n");
}


// Fonction pour charger les images des pièces
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        printf("Erreur chargement image %s: %s\n", path, IMG_GetError());
    }
    return texture;
}


// Fonction pour dessiner l'échiquier
void drawBoard(SDL_Renderer* renderer) {
    SDL_Rect tile;
    tile.w = TILE_SIZE;
    tile.h = TILE_SIZE;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255); // Case claire
            } else {
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255); // Case foncée
            }
            tile.x = j * TILE_SIZE;
            tile.y = i * TILE_SIZE;
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

// Fonction pour dessiner les pièces sur l'échiquier
void drawPieces(SDL_Renderer* renderer, SDL_Texture* pieces[12]) {
    SDL_Rect pieceRect = {0, 0, TILE_SIZE, TILE_SIZE};
    char piecesMap[12] = {'P', 'p', 'R', 'r', 'N', 'n', 'B', 'b', 'Q', 'q', 'K', 'k'};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 12; k++) {
                if (board[i][j] == piecesMap[k]) {
                    if (!pieces[k]) {  // Vérifie si la texture est bien chargée
                        printf("Erreur: texture %c non chargée\n", piecesMap[k]);
                        continue;
                    }
                    pieceRect.x = j * TILE_SIZE;
                    pieceRect.y = i * TILE_SIZE;
                    SDL_RenderCopy(renderer, pieces[k], NULL, &pieceRect);
                }
            }
        }
    }
}