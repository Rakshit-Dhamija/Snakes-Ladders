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
    std::cout << "- Type 'q' to quit" << std::endl << std::endl;
    
    // Create a board representation
    std::map<int, int> jumpTable;
    initializeBoard(jumpTable);
    
    std::vector<std::vector<std::string>> board(BOARD_SIZE, std::vector<std::string>(BOARD_SIZE, "     "));
    std::vector<std::vector<std::string>> playerMarkers(BOARD_SIZE, std::vector<std::string>(BOARD_SIZE, "     "));
    std::vector<std::vector<std::string>> snakesLadders(BOARD_SIZE, std::vector<std::string>(BOARD_SIZE, "     "));
    
    // Place snakes and ladders on the board
    for (const auto& jump : jumpTable) {
        int start = jump.first;
        int end = jump.second;
        
        int startRow = (start - 1) / 10;
        int startCol = (start - 1) % 10;
        
        // Adjust for zigzag pattern
        if (startRow % 2 == 1) {
            startCol = 9 - startCol;
        }
        
        if (start < end) {
            snakesLadders[9 - startRow][startCol] = " L>" + std::to_string(end);
        } else {
            snakesLadders[9 - startRow][startCol] = " S>" + std::to_string(end);
        }
    }
    
    // Place position numbers on the board
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        int position = i + 1;
        int row = i / 10;
        int col = i % 10;
        
        // Adjust for zigzag pattern
        if (row % 2 == 1) {
            col = 9 - col;
        }
        
        std::string posStr = std::to_string(position);
        if (position < 10) {
            posStr = " " + posStr + " ";
        } else if (position < 100) {
            posStr = " " + posStr;
        }
        
        board[9 - row][col] = posStr;
    }
    
    // Place players on the board
    for (size_t i = 0; i < playerPositions.size(); ++i) {
        int position = playerPositions[i];
        if (position > 0 && position <= 100) {
            int row = (position - 1) / 10;
            int col = (position - 1) % 10;
            
            // Adjust for zigzag pattern
            if (row % 2 == 1) {
                col = 9 - col;
            }
            
            // Add player marker
            playerMarkers[9 - row][col] += "  P" + std::to_string(i + 1);
        }
    }
    
    // Print the board
    std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << std::endl;
    for (size_t i = 0; i < board.size(); ++i) {
        // First line: position numbers
        for (size_t j = 0; j < board[i].size(); ++j) {
            std::cout << "|" << board[i][j];
        }
        std::cout << "|" << std::endl;
        
        // Second line: player positions
        for (size_t j = 0; j < board[i].size(); ++j) {
            std::cout << "|";
            if (playerMarkers[i][j] != "     ") {
                if (i == 9 && j == 0) { // Position 1
                    std::cout << RED << playerMarkers[i][j] << RESET;
                } else {
                    if (playerMarkers[i][j].find("P1") != std::string::npos) {
                        std::cout << RED << "  P1" << RESET;
                    }
                    if (playerMarkers[i][j].find("P2") != std::string::npos) {
                        std::cout << BLUE << "  P2" << RESET;
                    }
                }
            } else {
                std::cout << "     ";
            }
        }
        std::cout << "|" << std::endl;
        
        // Third line: snakes and ladders
        for (size_t j = 0; j < board[i].size(); ++j) {
            std::cout << "|";
            if (snakesLadders[i][j].find("L>") != std::string::npos) {
                std::cout << GREEN << snakesLadders[i][j] << RESET;
            } else if (snakesLadders[i][j].find("S>") != std::string::npos) {
                std::cout << RED << snakesLadders[i][j] << RESET;
            } else {
                std::cout << "     ";
            }
        }
        std::cout << "|" << std::endl;
        
        std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << std::endl;
    }
    
    // Display player positions and current turn
    std::cout << RED << "Player 1: Position " << playerPositions[0] << RESET << " | " 
              << BLUE << "Player 2: Position " << playerPositions[1] << RESET << std::endl;
    std::cout << "Current turn: " << (currentPlayer == 0 ? RED : BLUE) << "Player " << currentPlayer + 1 << RESET << std::endl;
    
    if (lastRoll > 0) {
        std::cout << "Last roll: " << lastRoll << std::endl << std::endl;
    }
}

int rollDice() {
    return rand() % 6 + 1;
}

void movePlayer(std::vector<int>& playerPositions, int playerIndex, int steps) {
    int currentPos = playerPositions[playerIndex];
    int targetPos = currentPos + steps;
    
    if (targetPos <= WINNING_POSITION) {
        playerPositions[playerIndex] = targetPos;
    }
}

bool checkWin(const std::vector<int>& playerPositions) {
    for (int position : playerPositions) {
        if (position == WINNING_POSITION) {
            return true;
        }
    }
    return false;
}

void animateMovement(std::vector<int>& playerPositions, int playerIndex, int steps, std::map<int, int>& jumpTable) {
    int currentPos = playerPositions[playerIndex];
    
    // Move one step at a time
    for (int i = 1; i <= steps; ++i) {
        currentPos++;
        playerPositions[playerIndex] = currentPos;
        
        // Show each movement step
        std::cout << "Player " << playerIndex + 1 << " moved to position " << currentPos << std::endl;
        delay(SLEEP_DURATION / 2);
    }
    
    // Check if player landed on a snake or ladder
    if (jumpTable.find(currentPos) != jumpTable.end()) {
        int newPos = jumpTable[currentPos];
        
        if (newPos > currentPos) {
            std::cout << GREEN << "Yay! Climbing up a ladder from " << currentPos << " to " << newPos << RESET << std::endl;
        } else {
            std::cout << RED << "Oops! Sliding down a snake from " << currentPos << " to " << newPos << RESET << std::endl;
        }
        
        delay(SLEEP_DURATION);
        playerPositions[playerIndex] = newPos;
    }
}

bool isSnake(int position, const std::map<int, int>& jumpTable) {
    auto it = jumpTable.find(position);
    return (it != jumpTable.end() && it->second < position);
}

bool isLadder(int position, const std::map<int, int>& jumpTable) {
    auto it = jumpTable.find(position);
    return (it != jumpTable.end() && it->second > position);
}

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

std::string getPlayerToken(int playerIndex) {
    switch(playerIndex) {
        case 0: return RED + "P1" + RESET;
        case 1: return BLUE + "P2" + RESET;
        default: return "??";
    }
}