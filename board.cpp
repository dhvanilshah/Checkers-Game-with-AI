#include "game.h"

using namespace std;

board::board(){
    for (int i = 0; i != 3; ++i){
		for (int j = 0; j != 4; ++j){
			currentGameBoard[i][j] = Player1Piece;}}
	for (int i = 3; i != 5; ++i){
		for (int j = 0; j != 4; ++j){
			currentGameBoard[i][j] = EmptySpot;}}
	for (int i = 5; i != 8; ++i){
		for (int j = 0; j != 4; ++j){
			currentGameBoard[i][j] = Player2Piece;}}
}

board::board(const board& b){
	for (int i = 0; i != 8; i++)
		for (int j = 0; j != 4; j++)
			currentGameBoard[i][j] = b.currentGameBoard[i][j];
}

board::board(ifstream &saveFile){
	string line;
	int i = 0;
	while (i != 8 && getline(saveFile, line))
	{
		line.erase(remove_if(line.begin(), line.end(),
		 [](char c) { return isspace(c); }), line.end());
		
		int j = 0;
		char charPiece = ' ';
		for (char piece : line) {
			switch (piece) {
				case '0':
				charPiece = EmptySpot;
				break;
				case '1':
				charPiece = Player1Piece;
				break;
				case '2':
				charPiece = Player2Piece;
				break;
				case '3':
				charPiece = Player1King;
				break;
				case '4':
				charPiece = Player2King;
				break;
				default:
				cout << "Your input file is invalid. Exiting program." << endl;
				exit(0);
				break;
			};
			currentGameBoard[i][j] = charPiece;
			++j;
		}
		++i;
	}
}

void board::renderBoard(){
	for (int i = 0; i < 8; ++i) {
		cout << BOARD_Y_LABELS[i] << "|";
        for (int j = 0; j < 4; ++j) {
			string game_char;

			switch (currentGameBoard[i][j]) {
				case EmptySpot:
				game_char = EmptyWhiteSpotDisplayCode;
				break;
				case Player1Piece:
				game_char = Player1PieceDisplayCode;
				break;
				case Player2Piece:
				game_char = Player2PieceDisplayCode;
				break;
				case Player1King:
				game_char = Player1KingDisplayCode;
				break;
				case Player2King:
				game_char = Player2KingDisplayCode;
				break;
				default:
				game_char = EmptyWhiteSpotDisplayCode;
				break;
			};

            if (i % 2 == 1) {
                cout << game_char << EmptyBlackSpotDisplayCode;
            } else {
                cout << EmptyBlackSpotDisplayCode << game_char;
            }
        }
        cout << endl;
    }
	cout << "   A  B  C  D  E  F  G  H " << endl;
	cout << endl;
}

void board::handleKinging(int& i, int& j){
    if (i == 0 && currentGameBoard[i][j] == Player2Piece){
        currentGameBoard[i][j] = Player2King;
	}
    if (i == 7 && currentGameBoard[i][j] == Player1Piece){
		currentGameBoard[i][j] = Player1King;
	}
}