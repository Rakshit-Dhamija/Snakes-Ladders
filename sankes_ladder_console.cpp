#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>


// ANSI color codes for terminal output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define CLEAR_SCREEN "\033[H\033[2J\033[3J"
// Game constants
const int BOARD_SIZE = 10;
const int WINNING_POSITION = 100;
const int SLEEP_DURATION = 500; // milliseconds between animation frames
// Function prototypes
void initializeBoard(std::map<int, int>& jumpTable);
void displayBoard(const std::vector<int>& playerPositions, int currentPlayer, int lastRoll = 0);
int rollDice();
void movePlayer(std::vector<int>& playerPositions, int playerIndex, int steps);
bool checkWin(const std::vector<int>& playerPositions);
void animateMovement(std::vector<int>& playerPositions, int playerIndex, int steps, std::map<int, int>& jumpTable);
bool isSnake(int position, const std::map<int, int>& jumpTable);
bool isLadder(int position, const std::map<int, int>& jumpTable);
void delay(int milliseconds);
std::string getPlayerToken(int playerIndex);
int main() {
    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Initialize game state
    std::vector<int> playerPositions(2, 0); // Two players starting at position 0
    int currentPlayer = 0;
    std::map<int, int> jumpTable;
    
    // Create snakes and ladders
    initializeBoard(jumpTable);
    
    // Game loop
    bool gameRunning = true;
    while (gameRunning) {
        // Display current state
        displayBoard(playerPositions, currentPlayer);
        
        // Player input
        std::cout << "Player " << currentPlayer + 1 << ", press Enter to roll the dice (or 'q' to quit): ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "q" || input == "Q") {
            gameRunning = false;
            continue;
        }
        
        // Roll dice
        int roll = rollDice();
        std::cout << YELLOW << "Player " << currentPlayer + 1 << " rolled a " << roll << "!" << RESET << std::endl;
        
        // Check if roll would exceed 100
        if (playerPositions[currentPlayer] + roll <= WINNING_POSITION) {
            // Move player with animation
            std::cout << "Moving player " << currentPlayer + 1 << "..." << std::endl;
            animateMovement(playerPositions, currentPlayer, roll, jumpTable);
            
            // Check for win
            if (checkWin(playerPositions)) {
                std::cout << CLEAR_SCREEN;
                displayBoard(playerPositions, currentPlayer, roll);
                std::cout << GREEN << "Congratulations! Player " << currentPlayer + 1 << " wins!" << RESET << std::endl;
                std::cout << "Press Enter to start a new game or 'q' to quit: ";
                std::getline(std::cin, input);
                
                if (input == "q" || input == "Q") {
                    gameRunning = false;
                } else {
                    // Reset the game
                    playerPositions[0] = playerPositions[1] = 0;
                    currentPlayer = 0;
                }
                continue;
            }
            
          
        } else {
            std::cout << "Can't move! You need exactly " 
                      << (WINNING_POSITION - playerPositions[currentPlayer]) 
                      << " to win. Skipping turn." << std::endl;
            delay(2000);
        }
        
        // Switch to next player
        currentPlayer = (currentPlayer + 1) % 2;
    }
    
    std::cout << "Thank you for playing Snakes and Ladders!" << std::endl;
    return 0;
}
void initializeBoard(std::map<int, int>& jumpTable) {
    // Add snakes (from head to tail)
    jumpTable[17] = 7;   // Snake from 17 to 7
    jumpTable[54] = 31;  // Snake from 54 to 31
    jumpTable[62] = 19;  // Snake from 62 to 19
    jumpTable[87] = 36;  // Snake from 87 to 36
    jumpTable[95] = 24;  // Snake from 95 to 24
    jumpTable[99] = 5;   // Snake from 99 to 5
    
    // Add ladders (from bottom to top)
    jumpTable[2] = 23;   // Ladder from 2 to 23
    jumpTable[8] = 34;   // Ladder from 8 to 34
    jumpTable[20] = 77;  // Ladder from 20 to 77
    jumpTable[32] = 68;  // Ladder from 32 to 68
    jumpTable[41] = 79;  // Ladder from 41 to 79
    jumpTable[74] = 92;  // Ladder from 74 to 92
}
void displayBoard(const std::vector<int>& playerPositions, int currentPlayer, int lastRoll) {
    std::cout << CLEAR_SCREEN;
    std::cout << CYAN << "=== SNAKES AND LADDERS ===" << RESET << std::endl << std::endl;
    
    std::cout << "Instructions:" << std::endl;
    std::cout << "- Roll the dice on your turn to move your token" << std::endl;
    std::cout << "- Ladders (L>) will take you up the board" << std::endl;
    std::cout << "- Snakes (S>) will take you down the board" << std::endl;
    std::cout << "- First player to reach position 100 wins!" << std::endl;
    std::cout << "- P1: Player 1, P2: Player 2" << std::endl << std::endl;
    
    std::cout << "Controls:" << std::endl;
    std::cout << "- Press Enter to roll the dice" << std::endl;
    
[truncated]
[truncated]