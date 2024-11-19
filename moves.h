#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include <list>

using namespace std;

class jump {
    public:
    int startI, startJ;
	int endI, endJ; 
    jump(int iStart, int jStart, int iEnd, int jEnd):
        startI(iStart), startJ(jStart), endI(iEnd), endJ(jEnd){};
};

class playerMove {
    public:
	string moveString;
    shared_ptr<board> finalBoard;

    playerMove(string moveString, shared_ptr<board> finalBoard): 
        moveString(moveString), finalBoard(finalBoard){};
};


class movesEngine {
    public: 
    movesEngine();

    // returns false if no moves are available
    bool getValidMovesForPlayer(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, char &currentPlayer);
    
    private:
    bool getJumps(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, char &currentPlayer);
    bool recursiveCreateJump(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, int &i, int &j, vector<shared_ptr<jump>> &jumps);
    bool checkSpotAndProcessJump(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, int iStart, int jStart, int iJumped, int jJumped, int iEnd, int jEnd, vector<shared_ptr<jump>> &jumps);
    bool checkJump(shared_ptr<board> &gameBoard, int &iStart, int &jStart, int &iJumped, int &jJumped, int &iEnd, int &jEnd);
    string generateJumpString(vector<shared_ptr<jump>> &jumps);

    bool getMoves(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, char &currentPlayer);
    void checkSpotAndCreateMove(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, int &iStart, int &jStart, int iEnd, int jEnd);

    bool isValidSpotOnBoard(int &i, int &j);
    string ijToStr(int &i, int &j);
};