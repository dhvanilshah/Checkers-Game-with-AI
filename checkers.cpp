// Dhvanil Shah
// Checkers Game with a playable AI
#include <iostream>
#include <sstream>
#include <string> 

#include "game.h"

using namespace std;

int askToLoadGame() {
    string userInput;
    char choice;
    bool newGame = true;

    while (true) {
        cout << "Load a game from a save file? (y/n): ";
        cin >> userInput;
        while (cin.get() != '\n');
        istringstream iss(userInput);
        iss >> choice;

        if (choice == 'y') {
            break;
        }
        else if (choice == 'n')
        {
            newGame = false;
            break;
        }
        else {
            cout << "\nInvalid input. Please try again.\n" << endl;
        }
    };
    cout << endl;

    return newGame;
}

bool promptPlayerMode(int id) {
    string userInput;
    bool cpu = true;
    char choice;
    while (true) {
        cout << "Will player " << id << " be a computer? (y/n): ";      
        cin >> userInput;
        while (cin.get() != '\n');
        istringstream iss(userInput);
        iss >> choice;

        if(choice != 'y' && choice != 'n') {
            cout << "\nInvalid input. Please try again.\n" << endl;
        } else if(choice == 'y') {
            break;
        } else {
            cpu = false;
            break;
        }
    };
    cout << endl;

    return cpu;
}

int promptAITimeLimit() {
    string userInput;
    int choice;
    while (true) {
        cout << "Enter a time limit for the computer in seconds (3-60): ";      

        cin >> userInput;
        while (cin.get() != '\n');
        istringstream iss(userInput);
        iss >> choice;

        if(choice > 60 || choice < 3) {
            cout << "\nInvalid input. Please try again.\n" << endl;
        } else {
            break;
        }
    };
    cout << endl;

    return choice;
}

bool getIsPlayer1TurnFromFile(ifstream &saveFile){
    string line;
    getline(saveFile, line);
    try {
        int result = stoi(line);
        if(result != 1 && result != 2){
            cout << "Your input file has an invalid value for player's turn. Exiting the program." << endl;
            exit(1);
        }
        return result == 1;
    } catch (const invalid_argument& e) {
        cout << "Your input file has an invalid value for player's turn: " << e.what() << ". Exiting the program." << endl;
        exit(1);
    } catch (const out_of_range& e) {
        cout << "Your input file has an out of range value for player's turn: " << e.what() << ". Exiting the program." << endl;
        exit(1);
    }
}

int getAITimeLimitFromFile(ifstream &saveFile){
    string line;
    getline(saveFile, line);
    try {
        int result = stoi(line);
        if(result > 60 || result < 3) {
            cout << "Your input file has a value for the computer time limit that is not within (3,60). Exiting the program." << endl;
        }
        return result;
    } catch (const invalid_argument& e) {
        cout << "Your input file has an invalid value for the computer time limit : " << e.what() << ". Exiting the program" << endl;
        exit(1);
    } catch (const out_of_range& e) {
        cout << "Your input file has an out of range value for the computer time limit : " << e.what() << ". Exiting the program" << endl;
        exit(1);
    }
}

int main() {
    bool isPlayer1CPU, isPlayer2CPU, isPlayer1Turn = true, alreadyGotAITimeLimit = false;
    int aiTimeLimit = 0;
    shared_ptr<board> gameboard;

    bool loadSavedGame = askToLoadGame();

    if (loadSavedGame){
        string filename;
        cout << "Enter the save file path: ";
        cin >> filename;
        cout << endl;

        ifstream fin(filename.c_str());

        gameboard = make_shared<board>(fin);
        isPlayer1Turn = getIsPlayer1TurnFromFile(fin);
        aiTimeLimit = getAITimeLimitFromFile(fin);
        alreadyGotAITimeLimit = true;
    } else {
        gameboard = make_shared<board>();
    }

    isPlayer1CPU = promptPlayerMode(1);
    isPlayer2CPU = promptPlayerMode(2);

    if(!alreadyGotAITimeLimit && (isPlayer1CPU || isPlayer2CPU)) {
        aiTimeLimit = promptAITimeLimit();
        alreadyGotAITimeLimit = true;
    }

     game game(gameboard, isPlayer1CPU, isPlayer2CPU, aiTimeLimit, isPlayer1Turn);

     game.playGame();
    
    return 0;
}
