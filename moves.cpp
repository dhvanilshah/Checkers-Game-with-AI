#include "game.h"

movesEngine::movesEngine(){
    return;
}

// Function returns false if there are no moves available.
bool movesEngine::getValidMovesForPlayer(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, char &currentPlayer){
    if(getJumps(movesForTurn, gameBoard, currentPlayer)){
        return true;
    } else if(getMoves(movesForTurn, gameBoard, currentPlayer)){
        return true;
    } else {
        return false;
    }
}

bool movesEngine::getJumps(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, char &currentPlayer){
    bool isPlayer1Turn = currentPlayer == Player1;
    int i = isPlayer1Turn ? 0 : 7;
    int endIndex = isPlayer1Turn ? 8 : -1;
    int movementDirection = isPlayer1Turn ? 1 : -1;

    for(; i != endIndex; i += movementDirection){
        for(int j = 0; j != 4; j++){
            char currentSpot = gameBoard->currentGameBoard[i][j];
            bool isCurrentPositionPlayerPiece = tolower(currentSpot) == currentPlayer;
            if(isCurrentPositionPlayerPiece){
                vector<shared_ptr<jump>> jumps;
                recursiveCreateJump(movesForTurn, gameBoard, i, j, jumps);
            }
        }
    }

    return movesForTurn.size() > 0;
}

bool movesEngine::recursiveCreateJump(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, int &i, int &j, vector<shared_ptr<jump>> &jumps) {
    char currentSpotPiece = gameBoard->currentGameBoard[i][j];
    bool jumpFound = false;
    if(tolower(currentSpotPiece) == Player1Piece || currentSpotPiece == Player2King){
        if(i % 2 == 0){
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i+1, j, i+2, j-1, jumps);
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i+1, j+1, i+2, j+1, jumps);
        } else {
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i+1, j, i+2, j+1, jumps);
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i+1, j-1, i+2, j-1, jumps);
        }
    }
    if(tolower(currentSpotPiece) == Player2Piece || currentSpotPiece == Player1King){
        if(i % 2 == 0){
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i-1, j, i-2, j-1, jumps);
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i-1, j+1, i-2, j+1, jumps);
        } else {
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i-1, j, i-2, j+1, jumps);
            jumpFound |= checkSpotAndProcessJump(movesForTurn, gameBoard, i, j, i-1, j-1, i-2, j-1, jumps);
        }
    }
    return jumpFound;
}

bool movesEngine::checkSpotAndProcessJump(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, int iStart, int jStart, int iJumped, int jJumped, int iEnd, int jEnd, vector<shared_ptr<jump>> &jumps){
    char currentSpotPiece = gameBoard->currentGameBoard[iStart][jStart];
    if(checkJump(gameBoard, iStart, jStart, iJumped, jJumped, iEnd, jEnd)){
                // if a jump is available
                // make a copy of the board and apply the jump
                // see if another jump is available
                shared_ptr<board> copiedGameBoard = make_shared<board>(*gameBoard);
                copiedGameBoard->currentGameBoard[iStart][jStart] = EmptySpot;
                copiedGameBoard->currentGameBoard[iJumped][jJumped] = EmptySpot;
                copiedGameBoard->currentGameBoard[iEnd][jEnd] = currentSpotPiece;

                vector<shared_ptr<jump>> jumpsCopy;
                for (shared_ptr<jump> jmp : jumps) {
                    jumpsCopy.push_back(make_shared<jump>(jmp->startI, jmp->startJ, jmp->endI, jmp->endJ));
                }
                jumpsCopy.push_back(make_shared<jump>(iStart, jStart, iEnd, jEnd));

                bool anotherJumpFound = recursiveCreateJump(movesForTurn, copiedGameBoard, iEnd, jEnd, jumpsCopy);

                if(!anotherJumpFound){
                    copiedGameBoard->handleKinging(iEnd, jEnd);
                    movesForTurn.push_back(make_shared<playerMove>(generateJumpString(jumpsCopy), copiedGameBoard));
                }

                // Clean up jump list
                jumpsCopy.clear();

                return true;
            } else {
                return false;
            }
}

bool movesEngine::checkJump(shared_ptr<board> &gameBoard, int &iStart, int &jStart, int &iJumped, int &jJumped, int &iEnd, int &jEnd){
    char opposingPlayer = tolower(gameBoard->currentGameBoard[iStart][jStart]) == Player1 ? Player2 : Player1;
    bool jumpSpotIsValid = isValidSpotOnBoard(iJumped, jJumped);
    bool jumpSpotIsEnemy = tolower(gameBoard->currentGameBoard[iJumped][jJumped]) == opposingPlayer;
    bool landSpotIsValid = isValidSpotOnBoard(iEnd, jEnd);
    bool landSpotIsEmpty = gameBoard->currentGameBoard[iEnd][jEnd] == EmptySpot;
    
    return jumpSpotIsValid && jumpSpotIsEnemy && landSpotIsValid && landSpotIsEmpty;
}

string movesEngine::generateJumpString(vector<shared_ptr<jump>> &jumps){
    string jumpString = string();
    bool processedFirstJump = false;
    for (shared_ptr<jump> jmp : jumps) {
        if(!processedFirstJump){
           jumpString += ijToStr(jmp->startI, jmp->startJ);
           processedFirstJump = true;
        }
        jumpString += " -> " + ijToStr(jmp->endI, jmp->endJ);
    }
    return jumpString;
}

bool movesEngine::getMoves(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, char &currentPlayer){
    bool isPlayer1Turn = currentPlayer == Player1;
    int i = isPlayer1Turn ? 7 : 0;
    int endIndex = isPlayer1Turn ? -1 : 8;
    int movementDirection = isPlayer1Turn ? 1 : -1;

    for(; i != endIndex; i -= movementDirection){
        for(int j = 0; j != 4; j++){
            bool isCurrentPositionPlayerPiece = gameBoard->currentGameBoard[i][j] == currentPlayer;
            bool isCurrentPositionPlayerKing = gameBoard->currentGameBoard[i][j] == toupper(currentPlayer);
            if (isCurrentPositionPlayerPiece || isCurrentPositionPlayerKing){
                // Check appropriate surrounding spots for opponenets
                // if any type of piece, check to see if moves in relative "forward" direction is available
                // if king piece, check to see if moves in relative "backwards" direction is available   
                char currentSpot = gameBoard->currentGameBoard[i][j];
                if(tolower(currentSpot) == Player1Piece || currentSpot == Player2King){
                    if(i % 2 == 0){
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i+1, j);
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i+1, j+1);
                    } else {
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i+1, j);
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i+1, j-1);
                    }
                } 

                if(tolower(currentSpot) == Player2Piece || currentSpot == Player1King){
                    if(i % 2 == 0){
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i-1, j);
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i-1, j+1);
                    } else {
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i-1, j);
                        checkSpotAndCreateMove(movesForTurn, gameBoard, i, j, i-1, j-1);
                    }
                } 
            }
        }
    }
    return movesForTurn.size() > 0;
}

void movesEngine::checkSpotAndCreateMove(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &gameBoard, int& iStart, int& jStart, int iEnd, int jEnd)
{
	if (isValidSpotOnBoard(iEnd, jEnd) && gameBoard->currentGameBoard[iEnd][jEnd] == EmptySpot)
	{
        string moveString = string();
        moveString += ijToStr(iStart, jStart) + " -> " + ijToStr(iEnd, jEnd);

        char currentSpotPiece = gameBoard->currentGameBoard[iStart][jStart];
        shared_ptr<board> copiedGameBoard = make_shared<board>(*gameBoard);
        copiedGameBoard->currentGameBoard[iStart][jStart] = EmptySpot;
        copiedGameBoard->currentGameBoard[iEnd][jEnd] = currentSpotPiece;
        copiedGameBoard->handleKinging(iEnd, jEnd);
        movesForTurn.push_back(make_shared<playerMove>(moveString, copiedGameBoard));
	}
}

bool movesEngine::isValidSpotOnBoard(int &i, int &j){
    return (i >= 0 && i < 8 && j >= 0 && j < 4);
}

string movesEngine::ijToStr(int &i, int &j){
    string coordinateString = string() + '(' + BOARD_X_LABELS[(2*j + (1 - (i%2)))] + ',' + BOARD_Y_LABELS[i] + ')';
    return coordinateString;
}
