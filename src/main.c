#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define TILE_SIZE 80

void displayEndScreen(SDL_Renderer* renderer, const char* message, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255, 255}; 
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = {
        (SCREEN_WIDTH - textWidth) / 2,
        (SCREEN_HEIGHT - textHeight) / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(textTexture);
}

void getMousePosition(int *x, int *y) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    *x = mouseX / TILE_SIZE;
    *y = mouseY / TILE_SIZE;
}


void drawSelectedTile(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 215, 255, 226);
    SDL_RenderFillRect(renderer, &rect);
}

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR OF INITIALISATION WITH SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("ERROR OF INITIALISATION WITH SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("ERROR OF INITIALISATION WITH SDL_ttf: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window* window = SDL_CreateWindow("Échecs SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("CREATION WINDOW: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("ERROR WITH RENDERER: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
    if (!font) {
        printf("FONT ERROR: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* pieces[12];
    const char* pieceFiles[12] = {
        "assets/bp.png", "assets/wp.png", "assets/br.png", "assets/wr.png",
        "assets/bn.png", "assets/wn.png", "assets/bb.png", "assets/wb.png",
        "assets/bq.png", "assets/wq.png", "assets/bk.png", "assets/wk.png"
    };

    for (int i = 0; i < 12; i++) {
        pieces[i] = loadTexture(renderer, pieceFiles[i]);
        if (!pieces[i]) {
            printf("Error: CAN'T CHANGE TEXTURE %s.\n", pieceFiles[i]);
            for (int j = 0; j < i; j++) {
                SDL_DestroyTexture(pieces[j]);
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_CloseFont(font);
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
    }

    char board[8][8];
    initializeBoard(board);

    int gameOver = 0;
    int currentPlayer = 1; 

    GamePosition selectedPiece = {-1, -1};

    SDL_Event event;
    while (!gameOver) {
        bool validMove = false;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameOver = 1;
                break;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x, y;
                getMousePosition(&x, &y);

                if (selectedPiece.position.x == -1) {
                    if (isPlayerPiece(board[y][x], currentPlayer)) {
                        selectedPiece.position.x = x;
                        selectedPiece.position.y = y;
                    }
                } else {
                    GamePosition to = {x, y};
                    if (isValidMove(board, selectedPiece, to, currentPlayer)) {
                        movePiece(board, selectedPiece, to, currentPlayer);
                        validMove = true;
                        selectedPiece.position.x = -1; 
                    } else {
                        if (isPlayerPiece(board[y][x], currentPlayer)) {
                            selectedPiece.position.x = x;
                            selectedPiece.position.y = y;
                        } else {
                            selectedPiece.position.x = -1;
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fond blanc
        SDL_RenderClear(renderer);

        drawBoard(renderer);
        drawPieces(renderer, pieces, board);

        if (selectedPiece.position.x != -1) {
            drawSelectedTile(renderer, selectedPiece.position.x, selectedPiece.position.y);
        }

        SDL_RenderPresent(renderer);

        bool isWhite = (currentPlayer == 1);
        Position kingPos = findKingPos(board, isWhite);

        if (isCheck(board, kingPos, isWhite)) {
            printf("Your king is in danger ! You must protect him\n");

            if (isCheckmate(board, isWhite)) {
                affichage(board);
                printf("Mate ! Player %d won.\n", currentPlayer);
                gameOver = 1;
                break;
            }
        }

        if (validMove) {
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }

        SDL_Delay(100);
    }

    if (gameOver) {
        const char* message;
        if (isCheckmate(board, currentPlayer == 1)) {
            message = (currentPlayer == 1) ? "The white Won !" : "The black won !";
        } else {
            message = "Game Over !";
        }

        displayEndScreen(renderer, message, font);

        SDL_Event endEvent;
        bool quitEndScreen = false;
        while (!quitEndScreen) {
            while (SDL_PollEvent(&endEvent)) {
                if (endEvent.type == SDL_QUIT || (endEvent.type == SDL_KEYDOWN && endEvent.key.keysym.sym == SDLK_ESCAPE)) {
                    quitEndScreen = true;
                }
            }
        }
    }

    for (int i = 0; i < 12; i++) {
        SDL_DestroyTexture(pieces[i]);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    printf("Good game ! Thanks for playing.\n");
    return 0;
}