#include "Player.h"
#include "Game.h"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

/**
 * FUNCTION: Prints the game board
 * PARAMS: VOID
 * RETURNS: VOID
 */
void TicTacToe::printGameBoard(void){
    #ifndef DEBUG
        for(int i = 0; i < 50; i++){
            std::cout << std::endl;
        }
    #endif 

    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m"; // Red color for X
    const std::string BLUE = "\033[34m"; // Blue color for O

    for (int i = 0; i < 9; i++) {
        if (slots[i] == 'X') {
            std::cout << RED << " X " << RESET;
        } else if (slots[i] == 'O') {
            std::cout << BLUE << " O " << RESET;
        } else {
            std::cout << " " << slots[i] << " " ;
        }
        // Print dividers
        if ((i + 1) % 3 == 0){
            std::cout << std::endl;
            if (i != 8) {          
                std::cout << "---+---+---" << std::endl;
            }
        } else {
            std::cout << "|";
        }      
    }
}

/**
 * FUNCTION: Prints the the player one's name vs player two's name
 * PARAMS: Pointers to both the player objects
 * RETURNS: VOID
 */
void TicTacToe::printTitle(){
    for(int i = 0; i < 50; i++){
        std::cout << std::endl;
    }

    /* Title */
    std::cout << playerOne->playerName << " vs. " << playerTwo->playerName << std::endl;
}

/** 
 * FUNCTION:    Determines who (Player 1 or 2) goes first
 * PARAMS:      List of players
 * RETURNS:     int value (0 or 1)
 */
int TicTacToe::determineWhoMovesFirst(){
    int userIn{-1};

    std::cout << "Which player goes first?\n";
    std::cout << "\t1. " << players[0]->playerName << std::endl;
    std::cout << "\t2. " << players[1]->playerName << std::endl;

    std::cin >> userIn;

    while(true){
        
        if(userIn == 1 || userIn == 2){
            return userIn - 1;
        }

        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(INT32_MAX, '\n');
        }

        std::cout << "Invalid Option." << std::endl;
        std::cin >> userIn;
    }
    
}

/**
 * FUNCTION: Given a slot number, updates the slot to the current player's symbol
 * PARAMS:   current player, p, and the slot number they chose
 * RETURNS:  VOID
 */
void TicTacToe::updateSlot(struct Player *p, int slotToUpdate){
    slots[slotToUpdate - 1] = p->playerSymbol;
}

/**
 * FUNCTION:    Asks for a user input on next move
 * PARAMS:      VOID
 * RETURNS:     An int value (1 - 9), that corresponds with a place on the TicTacToe board
 */
int TicTacToe::getPlayerMove(void){
    int userIn{-1};

    std::cout << "Enter in where to move: ";
    std::cin >> userIn; 

    /* Determine if input is valid and not already used */
    while((userIn < 1 || userIn > 9) || (std::cin.fail()) || (slots[userIn - 1] == 'X' || slots[userIn - 1] == 'O')){
        std::cin.clear();
        std::cin.ignore(INT32_MAX, '\n');
        
        std::cout << "Invalid Slot." << std::endl;
        std::cout << "Enter in where to move: ";
        std::cin >> userIn;
    }

    return userIn;
}

/**
 * FUNCTION:    Determines where the AI will move
 * PARAMS:      VOID
 * RETURNS:     Int value representing the slot where the AI is moving
 * TODO:        Add algorithms so the AI is choosing slots that aren't at random
 */
int TicTacToe::getAIMove(void){ 
    std::random_device rd;    
    unsigned int AIMove = rd();
    
    constexpr int MAX_SLEEP = 4; 
    int sleepTime = (AIMove % MAX_SLEEP) + 1; 
    std::this_thread::sleep_for(std::chrono::seconds(sleepTime));

    do{
        AIMove = rd();
        AIMove %= boardSize;

        #ifdef DEBUG
            std::cout << "AI Rolled: " << AIMove << std::endl;
        #endif

    }while((AIMove < 1 || AIMove > 10) || slots[AIMove - 1] == 'X' || slots[AIMove - 1] == 'O');

    #ifdef DEBUG
        std::cout << "AI MOVING TO: " << AIMove << std::endl;
    #endif 
    
    return AIMove;
}

/** 
 * FUNCTION: Determins if there is a winner 
 * PARAMS:   VOID
 * RETURNS:  1 -> Winner detected | 0 -> Winner not detected
 */
int TicTacToe::determineWinner(void) {
    // Horizontals
    if ((slots[0] == slots[1] && slots[1] == slots[2]) ||
        (slots[3] == slots[4] && slots[4] == slots[5]) ||
        (slots[6] == slots[7] && slots[7] == slots[8]) ||
        
        // Verticals
        (slots[0] == slots[3] && slots[3] == slots[6]) ||
        (slots[1] == slots[4] && slots[4] == slots[7]) ||
        (slots[2] == slots[5] && slots[5] == slots[8]) ||
        
        // Diagonals
        (slots[0] == slots[4] && slots[4] == slots[8]) ||
        (slots[2] == slots[4] && slots[4] == slots[6])) {
        return 1;
    }
    return 0;
}

/**
 * FUNCTION: Determines if there is a tie. After all moves are exhausted, thus 9 moves have been made, there must be a tie.
 * PARAMS:   Current rount
 * RETURNS:  True or false, depending if there is a tie or not.
 */
bool TicTacToe::determineTie(){
    if(++currentRound == 9){
        return true;
    } else {
        return false;
    }
}

/**
 * FUNCTION: Main function for playing the game
 * PARAMS:   Pointers to both player objects
 * RETURNS:  VOID
 */
void TicTacToe::playGame(){

    #ifdef DEBUG
        if(playerTwo->isPlayerAI){
            std::cout << "Player Two is AI." << std::endl;
        }
    #endif 

    /* Tracks the current player's (NPC | Player) move */
    int currentPlayerMove{-1};

    /* Player that will move first */
    int currentPlayerIndex = determineWhoMovesFirst();
    struct Player* currentPlayer = players[currentPlayerIndex];

    while(gameOn){
        printGameBoard();

        /* Lets the players know who's turn it is */
        std::cout << std::endl << currentPlayer->playerName << "'s Turn!" << std::endl;

        /* Branch depending if character is AI or not */
        currentPlayerMove = (currentPlayer->isPlayerAI) ? getAIMove() : getPlayerMove();

        updateSlot(currentPlayer, currentPlayerMove);
        
        /* Did someone win? */
        if(determineWinner()){
            printGameBoard();
            std::cout << std::endl << currentPlayer->playerName << " won!" << std::endl;
            gameOn = 0;
        
        /* Are all moves exhausted, thus a tie? */
        } else if (determineTie()){
            printGameBoard();
            std::cout << "TIE" << std::endl;
            gameOn = 0;

        /* No winner, nor tie, keep going*/
        } else {

            /* Other player's turn */
            currentPlayerIndex = (currentPlayerIndex + 1) % 2;
            currentPlayer = players[currentPlayerIndex];
        }

        #ifdef DEBUG
            std::cout << "==========================================================" << std::endl;
            std::cout << currentPlayerIndex << std::endl;
            std::cout << currentPlayer->playerName << std::endl; 
            std::cout << currentRound << std::endl;
            std::cout << "==========================================================" << std::endl;
        #endif
    }
}
