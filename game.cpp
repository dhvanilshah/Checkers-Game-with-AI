#include "game.h"


game::game(shared_ptr<board> gameBoard, bool p1Computer, bool p2Computer, int timeLimit, bool p1Turn){
    mainGameBoard = gameBoard;
    isPlayer1Computer = p1Computer;
    isPlayer2Computer = p2Computer;
    computerTimeLimit = timeLimit;
    isPlayer1Turn = p1Turn;
}

void game::playGame()
{
    vector<shared_ptr<playerMove>> movesForTurn;
    movesEng = movesEngine();
    int gameMovesCount = 0;

    while(true){
        char player = isPlayer1Turn ? Player1 : Player2;
        bool isComputerTurn = (isPlayer1Turn && isPlayer1Computer) || (!isPlayer1Turn && isPlayer2Computer);
        
        // Show the game board
        mainGameBoard->renderBoard();

        // Get Moves
        clearMovesList(movesForTurn);
        movesEng.getValidMovesForPlayer(movesForTurn, mainGameBoard, player);

        // Perform a terminal test
        if(movesForTurn.size() == 0){
            endGame();
            cout << "The game ended in " << gameMovesCount << " moves." << endl;
            exit(0);
        }

        if(isComputerTurn){
            // Say computer will choose a move and randomly choose a move and change turn
            getAndApplyAIMove(movesForTurn);
        } else {
            // List available moves, wait for user prompt, and change turn
            getAndApplyUserMove(movesForTurn);
        }
        gameMovesCount += 1;
        cout << endl;
    }
}

void game::getAndApplyUserMove(vector<shared_ptr<playerMove>> &movesForTurn){
    string player = isPlayer1Turn ? "Player 1" : "Player 2";
    cout << player << " to move." << endl;
    cout << "The legal moves are:" << endl;
    printMoves(movesForTurn);

    string userInput;
    int moveNumber;
    while (true) {
        cout << "Enter the number correspondig to the move: ";   

        cin >> userInput;
        while (cin.get() != '\n');
        istringstream iss(userInput);
        iss >> moveNumber;

        if(moveNumber > movesForTurn.size() || moveNumber < 1) {
            cout << "\nInvalid input. Please try again.\n" << endl;
        } else {
            break;
        }
    };

    shared_ptr<playerMove> selectedMove = movesForTurn.at(moveNumber - 1);
	
    // todo: manage memory better here
	mainGameBoard = selectedMove->finalBoard;
    changeTurn();

    cout << endl << player << " chose move " << moveNumber << ": ";
    cout << selectedMove->moveString << endl;
};

void game::getAndApplyAIMove(vector<shared_ptr<playerMove>> &movesForTurn){
    string player = isPlayer1Turn ? "Player 1" : "Player 2";
    cout << player << " to move." << endl;
    cout << "The legal moves are:" << endl;
    printMoves(movesForTurn);

    shared_ptr<playerMove> selectedMove;

    if (movesForTurn.size() == 1){
		selectedMove = movesForTurn.front();
		time(&computerMoveStartTime);
		time(&computerMoveEndTime);
		currentDepth = 0;
	}
	else{
        time(&computerMoveStartTime);

		for (int i = 1; i != MAX_IDS_DEPTH; ++i){
			time(&currentDepthSearchStartTime);

			maxDepth = i;

			alphabeta(movesForTurn, mainGameBoard, isPlayer1Turn, i, numeric_limits<int>::min(), numeric_limits<int>::max());

			time(&currentDepthSearchEndTime);

			//  From project spec: choosing to stop depth search if more than 1/2 the time has been used
			if (difftime(currentDepthSearchEndTime, currentDepthSearchStartTime) >= (computerTimeLimit/2))
			{
				time(&computerMoveEndTime);
				timeout = true;
				break;
			}

			if (timeout){
				break;
            } else {
				selectedMove = aiSearchRollingBestMove;
			}

			if(reachedEnd){
				break;
			}
		}
	}
	
	mainGameBoard = selectedMove->finalBoard;
    changeTurn();

    cout << endl << player << " chose move: ";
    cout << selectedMove->moveString << endl;
	cout << "The computer searched to depth " << maxDepth << " in " << difftime(computerMoveEndTime, computerMoveStartTime) << " seconds" << endl;
	if(reachedEnd){
		cout << "The search reached atleast one end node." << endl;
	}
	timeout = false;
    reachedEnd = false;
	selectedMove = NULL;
	aiSearchRollingBestMove = NULL;
};


int game::alphabeta(vector<shared_ptr<playerMove>> &movesForTurn, shared_ptr<board> &startingBoard, bool player1Turn, int depth, int alpha, int beta)
{
	if (depth != maxDepth && movesForTurn.size() == 0){
		reachedEnd = true;
		currentDepth = maxDepth;
		if (player1Turn){
            return std::numeric_limits<int>::min();
        } else {
            return std::numeric_limits<int>::max();
        }
	}
	reachedEnd = false;
	if (depth == 0){
		return heuristicEvaluation(startingBoard, player1Turn);
	}
	int localalpha = std::numeric_limits<int>::min();
	int localbeta = std::numeric_limits<int>::max();

	if(player1Turn){

		for (shared_ptr<playerMove> pm : movesForTurn)
		{
			time(&computerMoveEndTime);

			if (difftime(computerMoveEndTime, computerMoveStartTime) >= (computerTimeLimit - 1))
			{
				timeout = true;
				break;
			}


            vector<shared_ptr<playerMove>> movesForNextTurn;
            char nextPlayer = Player2;
            movesEng.getValidMovesForPlayer(movesForNextTurn, pm->finalBoard, nextPlayer);
			int value = alphabeta(movesForNextTurn, pm->finalBoard, false, depth-1, alpha, min(localbeta, beta));
            clearMovesList(movesForNextTurn);

			if (value > alpha)
			{
				alpha = value;
				if (depth == maxDepth)
					aiSearchRollingBestMove = pm;
			}

			if (alpha >= beta && depth < maxDepth)
				return alpha;
		}

		if (!timeout && depth == maxDepth)
			currentDepth = depth;

		return alpha;
	} else {
		for (shared_ptr<playerMove> pm : movesForTurn)
		{
			time(&computerMoveEndTime);
			if (difftime(computerMoveEndTime, computerMoveStartTime) >= (computerTimeLimit - 1))
			{
				timeout = true;
				break;
			}
			vector<shared_ptr<playerMove>> movesForNextTurn;
            char nextPlayer = Player1;
            movesEng.getValidMovesForPlayer(movesForNextTurn, pm->finalBoard, nextPlayer);
			int value = alphabeta(movesForNextTurn, pm->finalBoard, true, depth-1, max(localalpha, alpha), beta);
            clearMovesList(movesForNextTurn);

			if (value < beta)
			{
				beta = value;
				if (depth == maxDepth)
					aiSearchRollingBestMove = pm;
			}

			if (alpha >= beta)
				return beta;
		}

		if (!timeout && depth == maxDepth)
			currentDepth = depth;

		return beta;
	}
}


// Determnied Hueristic Priorities:
// 1. Count of all pieces where King/Pawn is 5/3
// 2. A Back Row Score where players guaring their back row or close to kinging are awarded
// 3. A piece count difference which determines who is dominating the board
// 4. A measurement near end game where double corners add a bonus for losing player and capturing those corners add a bonus for the winner
// 5. A pseudo-random score in the case that multiple moves tie for best
int game::heuristicEvaluation(shared_ptr<board> &board, bool player1Turn)
{
	int player1PieceCount = 0, player2PieceCount = 0;
	int backRowScore = 0;
	int boardDominancyScore = 0;
	int doubleCornerScore = 0;

	for (int i = 0; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
		{
			if (board->currentGameBoard[i][j] == Player1Piece)
			{
				player1PieceCount += 3;
				if (i == 0)
					backRowScore += 9;
				else backRowScore += i;
				boardDominancyScore += 1;
			}
			else if (board->currentGameBoard[i][j] == Player2Piece)
			{
				player2PieceCount -= 3;
				if (i == 7)
					backRowScore -= 9;
				else backRowScore -= (7 - i);
				boardDominancyScore -= 1;
			}
			else if (board->currentGameBoard[i][j] == Player1King)
			{
				player1PieceCount += 5;
				boardDominancyScore += 1;
			}
			else if (board->currentGameBoard[i][j] == Player2King)
			{
				player2PieceCount -= 5;
				boardDominancyScore -= 1;
			}
		}

	if (boardDominancyScore > 0 && player2PieceCount >= -10){
		doubleCornerScore -= cornerDiagonal(board, Player2, Player1);
	}
	else if (boardDominancyScore < 0 && player1PieceCount <= 10){
		doubleCornerScore += cornerDiagonal(board, Player1, Player2);
	}

	int totalPieceScore = (player1PieceCount + player2PieceCount) * 100000000;

	backRowScore *= 1000000;

	boardDominancyScore *= 10000;

	doubleCornerScore *= 100;

	int randomBonus = rand() % 100;
	randomBonus = player1Turn ? randomBonus : -randomBonus;

	return (totalPieceScore + backRowScore + boardDominancyScore + doubleCornerScore + randomBonus);
}

// Score for holding / capturing a double corner
int game::cornerDiagonal(shared_ptr<board> &board, char losingPlayer, char winningPlayer)
{
	int c = 0;
	if (tolower(board->currentGameBoard[0][0]) == losingPlayer || tolower(board->currentGameBoard[1][0]) == losingPlayer)
	{
		c += 9;
		if (tolower(board->currentGameBoard[0][0]) == winningPlayer)
			c -= 3;
		if (tolower(board->currentGameBoard[1][0]) == winningPlayer)
			c -= 3;
		if (tolower(board->currentGameBoard[1][1]) == winningPlayer)
			c -= 1;
		if (tolower(board->currentGameBoard[2][0]) == winningPlayer)
			c -= 1;
		if (tolower(board->currentGameBoard[2][1]) == winningPlayer)
			c -= 1;
		if (tolower(board->currentGameBoard[3][1]) == winningPlayer)
			c -= 1;
	}
	if (tolower(board->currentGameBoard[6][3]) == losingPlayer || tolower(board->currentGameBoard[7][3]) == losingPlayer)
	{
		c += 9;
		if (tolower(board->currentGameBoard[4][2]) == winningPlayer)
			c -= 1;
		if (tolower(board->currentGameBoard[5][2]) == winningPlayer)
			c -= 1;
		if (tolower(board->currentGameBoard[5][3]) == winningPlayer)
			c -= 1;
		if (tolower(board->currentGameBoard[6][2]) == winningPlayer)
			c -= 1;
		if (tolower(board->currentGameBoard[6][3]) == winningPlayer)
			c -= 3;
		if (tolower(board->currentGameBoard[7][3]) == winningPlayer)
			c -= 3;
	}
	return c;
}

void game::clearMovesList(vector<shared_ptr<playerMove>> &movesForTurn){
    movesForTurn.clear(); // Note: there was more here until smart pointers took over memory management
}

void game::printMoves(vector<shared_ptr<playerMove>> &movesForTurn){
    int index = 0;
    for (shared_ptr<playerMove> pm : movesForTurn) {
        cout << "[" << index+1 << "] " << pm->moveString << endl;
        index++;
    }
}

void game::changeTurn(){
    isPlayer1Turn = !isPlayer1Turn;
}

void game::endGame(){
    string player = isPlayer1Turn ? "PLAYER 2" : "PLAYER 1";
    cout << "The game is over." << endl;
	cout << player << " WINS!!" << endl;
}