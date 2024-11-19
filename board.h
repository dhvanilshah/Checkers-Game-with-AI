#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include <list>

using namespace std;

#define Player1 'b'
#define Player2 'r'

#define Player2Piece 'r'
#define Player2PieceDisplayCode "\x1b[48;5;255m\x1b[38;5;9m ● \x1b[0m"

#define Player1Piece 'b'
#define Player1PieceDisplayCode "\x1b[48;5;255m\x1b[38;5;0m ● \x1b[0m"

#define Player2King 'R'
#define Player2KingDisplayCode "\x1b[48;5;255m\x1b[38;5;9m\x1b[1m K \x1b[0m"

#define Player1King 'B'
#define Player1KingDisplayCode "\x1b[48;5;255m\x1b[38;5;0m\x1b[1m K \x1b[0m"

#define EmptySpot 'e'
#define EmptyWhiteSpotDisplayCode "\x1b[48;5;255m   \x1b[0m"
#define EmptyBlackSpotDisplayCode "\x1b[48;5;0m   \x1b[0m"

#define BOARD_Y_LABELS "87654321"
#define BOARD_X_LABELS "ABCDEFGH"
#define BOARD_HEIGHT 8
#define BOARD_WIDTH 4

class board {
    public: 
    char currentGameBoard[8][4];

    board();
    board(ifstream &saveFile);
    board(const board&);

    void renderBoard();
    void validMoves (unordered_map<int, list<int>> &moves, int symbol);
    void handleKinging(int& i, int& j);
    
    private:
};