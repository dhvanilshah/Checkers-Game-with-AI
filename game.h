#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <random>
#include "board.h"
#include "moves.h"

using namespace std;

#define MAX_IDS_DEPTH 100

class game {
    public: 
    bool isPlayer1Turn;

    game(shared_ptr<board> gameBoard, bool p1Computer, bool p2Computer, int timeLimit, bool p1Turn);

    void playGame();
    
    private:
    movesEngine movesEng;

    bool isPlayer1Computer;
    bool isPlayer2Computer;
    int computerTimeLimit;
    shared_ptr<board> mainGameBoard;

	time_t computerMoveStartTime;
	time_t computerMoveEndTime;

	time_t currentDepthSearchStartTime;
	time_t currentDepthSearchEndTime;

    int maxDepth;
    bool timeout;
    bool reachedEnd;
    int currentDepth;
    shared_ptr<playerMove> aiSearchRollingBestMove;
    
    void clearMovesList(vector<shared_ptr<playerMove>> &movesForTurn);
    void getAndApplyUserMove(vector<shared_ptr<playerMove>> &movesForTurn);
    void getAndApplyAIMove(vector<shared_ptr<playerMove>> &movesForTurn);
    int alphabeta(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &startingBoard, bool player1Turn, int depth, int alpha, int beta);
    int heuristicEvaluation(shared_ptr<board> &board, bool player1Turn);
    int cornerDiagonal(shared_ptr<board> &board, char losing, char winning);
    void printMoves(vector<shared_ptr<playerMove>> &movesForTurn);
    void changeTurn();
    void endGame();
};