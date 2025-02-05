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
void drawPieces(SDL_Renderer* renderer, SDL_Texture* pieces[12], char board[8][8]) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char piece = board[y][x];
            int index = -1;
            switch (piece) {
                case 'P': index = 0; break;  // Pion blanc
                case 'p': index = 1; break;  // Pion noir
                case 'R': index = 2; break;  // Tour blanche
                case 'r': index = 3; break;  // Tour noire
                case 'N': index = 4; break;  // Cavalier blanc
                case 'n': index = 5; break;  // Cavalier noir
                case 'B': index = 6; break;  // Fou blanc
                case 'b': index = 7; break;  // Fou noir
                case 'Q': index = 8; break;  // Reine blanche
                case 'q': index = 9; break;  // Reine noire
                case 'K': index = 10; break; // Roi blanc
                case 'k': index = 11; break; // Roi noir
            }
            if (index != -1 && pieces[index]) {
                SDL_Rect dest = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderCopy(renderer, pieces[index], NULL, &dest);
            }
        }
    }
}