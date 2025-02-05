# Chess Game

#### Video Demo:  <URL HERE>

## How to Run the Game

1. Clone or download the repository.
2. type in the terminal `gcc -o main src/main.c src/board.c src/game.c src/piece.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -framework Cocoa` and then `./main`

3. If you want to exit juste do `CTRL + C`


## Introduction

This project is a chess game. It requires two player and the goal is just to play a fun game of chess.
The code is organized into different files, each responsible for a specific part of the game. Below, I will provide a quick overview of the functionality contained in each file, along with explanations of the most important functions.

The files `nameFile.h` are the files where the function and the structure are declared. It helps with the fact that some functions can be used in another file.c .

#### File: `board.h` and `board.c`

The `board.c` file is responsible for the chessboard setup and display. This file includes the functions needed to initialize the board with the starting position of pieces and to display the board to the players.
- `initializeBoard(char board[8][8])`: This function sets up the initial state of the chessboard. It places all the pieces in their starting positions on the 8 x 8 grid. This is called at the beginning of the game to ensure the board is ready for play.
- `affichage(char board[8][8])`: This function prints the current state of the chessboard to the console, allowing players to visualize the game. It loops through the board, displaying each piece or empty space, and organizes the output in a format that reflects the chessboard.

#### File: `game.c` and `game.h`

The `game.c` file is where most of the logic for gameplay resides. This file manages player turns, validates moves, and transform the movement given from the player to an actual position on the board. 
In `game.h` we have `GamePosition`:, This structure represents a piece and its position on the chessboard. It contains two fields: Position(which holds the x and y coordinates) and Piece (which holds information about the piece type and color). This structure is useful when determining the location and identity of a piece on the board.
- `roundG(char board[8][8], int currentPlayer)`: This function runs a round of the game, where the current player is prompted to make a move. It ensures that the move is valid by checking the piece's movement rules and verifying that the target square is not blocked or illegal.

- `transformer(char* input)`: This function converts a string input (like "e2 e4") into a GamePosition structure. It breaks down the string into a starting position (from) and an ending position (to), making it easier to process the player's move.

- `isValidMove(char board[8][8], GamePosition from, GamePosition to, int currentPlayer)`: This function checks whether a move is valid based on the rules of chess. It uses the piece type and the current player's turn to verify that the piece's movement is legal and that there are no obstructions in the way.

- `movePiece(char board[8][8], GamePosition from, GamePosition to, int currentPlayer)`: This function actually moves a piece on the chessboard if the move is valid. It updates the board array by removing the piece from its initial position and placing it in its new position.

- `isPiecePresent(Piece piece, GamePosition pos)`: This function checks if a particular piece is present at the given position. It is useful for checking whether a square is occupied by another piece before making a move.


#### File: `piece.c`and `piece.h`

The `piece.c` file is responsible for managing the different types of chess pieces and their movement rules. Each piece has its own movement logic, and this file contains functions for validating those movements.

- `Piece`: The Piece structure contains two fields: type and color. The type represents the piece (e.g., 'P' for pawn, 'R' for rook), and color determines whether the piece is white or black (true for white, false for black).

- `isValidMovement(char board[8][8], Position from, Position to, Piece piece, int curr)`: This function checks whether a piece can legally move from a starting position (from) to a target position (to). 

- `movePion(Position from, Position to, Piece piece, int curr)`: This function checks whether a pawn's move is valid. A pawn has special movement rules, such as being able to move two squares on its first turn, moving forward by one square, or capturing diagonally.

- `moveTour(char board[8][8], Position from, Position to)`: This function checks whether a rook's move is valid. Rooks can move vertically or horizontally, as long as the path is clear from pieces of his own color.

-	`moveCavalier(Position from, Position to)`: This function checks whether a knight's move is valid. Knights move in an "L" shape, jumping over other pieces.

-	`moveFou(char board[8][8], Position from, Position to)`: This function checks whether a bishop's move is valid. Bishops can only move diagonally, and the path 
must be clear clear from pieces of his own color.

-	`moveReine(char board[8][8], Position from, Position to)`: This function checks whether a queen's move is valid. A queen can move both like a rook (vertically or horizontally) and like a bishop (diagonally), so it combines the rules for both 
pieces.

-	`moveRoi(char board[8][8], Position from, Position to)`: This function checks whether a king's move is valid. A king can move one square in any direction.	

-	`isPathClear(char board[8][8], Position from, Position to)`: This function checks whether the path between two positions is clear of other pieces. It's used by pieces that move in straight lines (e.g., rooks, bishops, and queens) to verify that no other pieces block their movement.

-	`bool isCheck(char board[8][8], Position kingPos, bool isWhite)`: This function checks whether the king is in a chack position

-	`bool isCheckmate(char board[8][8], bool isWhite)`: This function checks if there is a checkmate by seeing of the king can still move or be protected.

#### File: `position.h`
The `position.h` file defines the Position structure, which is used to represent the x and y coordinates of a square on the chessboard. The structure contains two fields: x (the column, ranging from 0 to 7) and y (the row, also ranging from 0 to 7). These fields are used to define the location of pieces on the chessboard.

#### File: `main.c`
This file serves as the entry point to the program and manages the game loop. It initializes the board, handles player turns, checks for check or checkmate, and displays the board after each move.
-	The main function initializes the board, then enters a game loop where each player takes turns making a move. After every move, the program checks if the player's king is in check or checkmate. If checkmate is detected, the game ends and announces the winner. After each turn, the game switches to the other player's turn.

## Conclusion

This chess game is built from multiple files that work together to provide an interactive chess game experience. The board file handles the setup and display of the chessboard, the game file manages the game logic, and the piece file defines the rules for each chess piece's movement. The position file provides the necessary structure to represent positions on the chessboard, while the main file ties everything together into a playable game.