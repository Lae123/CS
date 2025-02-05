#ifndef BOARD_H
#define BOARD_H
#include <SDL2/SDL_image.h>
void initializeBoard (char board[8][8]);
void affichage(char board[8][8]);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
void drawBoard(SDL_Renderer* renderer);
void drawPieces(SDL_Renderer* renderer, SDL_Texture* pieces[12], char board[8][8]);
#endif