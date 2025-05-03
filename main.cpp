// main.cpp for Snakes and Ladders Game

// Platform detection - we need to handle includes differently for Windows vs Linux/macOS
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h> // must come before GLUT on Windows
    #include <GL/freeglut.h>
#else
    // For Linux and macOS
    #include <GL/glut.h>
    #include <GL/freeglut.h>
#endif

#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
#include <iostream>

// Board & players
int jumpTable[101];        // snakes and ladders jump table
int playerPos[2] = {0,0};
int currentPlayer = 0;

// Animation
bool animating = false;
int animTarget = 0;
int diceRoll = 0;

// Window
const int WIN_W = 600, WIN_H = 600;
const int N = 10;          // 10×10 grid

// Initialize snakes & ladders
void initJumps() {
    for(int i=1;i<=100;i++) jumpTable[i]=0;
    // ladders
    jumpTable[2]=23;   jumpTable[8]=34;
    jumpTable[20]=77;  jumpTable[32]=68;
    jumpTable[41]=79;  jumpTable[74]=92;
    // snakes
    jumpTable[99]=5;   jumpTable[95]=24;
    jumpTable[87]=36;  jumpTable[62]=19;
    jumpTable[54]=31;  jumpTable[17]=7;
}

// Convert board index [1..100] to window coords
void boardToXY(int idx, float &x, float &y) {
    if(idx<1) { x=-10; y=-10; return; }
    int row = (idx-1)/N;
    int col = (idx-1)%N;
    if(row%2==1) col = N-1-col;
    x = col * (WIN_W/N);
    y = row * (WIN_H/N);
}

// Draw the 10×10 checkerboard
void drawBoard() {
    float w = WIN_W/(float)N, h = WIN_H/(float)N;
    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            bool bright = (r+c)%2==0;
            glColor3f(bright?0.9f:0.6f, bright?0.9f:0.6f,0.9f);
            glBegin(GL_QUADS);
               glVertex2f(c*w,   r*h);
               glVertex2f(c*w+w, r*h);
               glVertex2f(c*w+w, r*h+h);
               glVertex2f(c*w,   r*h+h);
            glEnd();
            // draw square number
            int idx = r*N + (r%2==0? c: (N-1-c)) + 1;
            glColor3f(0,0,0);
            std::string s = std::to_string(idx);
            glRasterPos2f(c*w+5, r*h+5);
            for(char ch:s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ch);
        }
    }
}

// Draw snakes (red) and ladders (green)
void drawJumps() {
    glLineWidth(4);
    for(int i=1;i<=100;i++){
        int j = jumpTable[i];
        if(j==0) continue;
        float x1,y1,x2,y2;
        boardToXY(i, x1,y1);
        boardToXY(j, x2,y2);
        // center of squares
        x1 += WIN_W/N/2;  y1 += WIN_H/N/2;
        x2 += WIN_W/N/2;  y2 += WIN_H/N/2;
        if(j>i) glColor3f(0,0.7f,0);
        else     glColor3f(0.7f,0,0);
        glBegin(GL_LINES);
           glVertex2f(x1,y1);
           glVertex2f(x2,y2);
        glEnd();
    }
}

// Draw a player's token as a filled circle
void drawToken(int player) {
    float x,y;
    boardToXY(playerPos[player], x,y);
    x += WIN_W/N/2;  y += WIN_H/N/2;
    float radius = WIN_W/N * 0.3f;
    if(player==0) glColor3f(0,0,1);
    else          glColor3f(1,0.5f,0);
    int tris = 20;
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(x,y);
      for(int i=0;i<=tris;i++){
        float a = i*2.0f*3.14159f/tris;
        glVertex2f(x + cosf(a)*radius, y + sinf(a)*radius);
      }
    glEnd();
}

// Draw the current dice roll
void drawDice() {
    if (diceRoll == 0) return;
    
    // Position at top-right corner
    float diceSize = 50.0f;
    float margin = 10.0f;
    float x = WIN_W - diceSize - margin;
    float y = WIN_H - diceSize - margin;
    
    // Draw dice background
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
       glVertex2f(x, y);
       glVertex2f(x + diceSize, y);
       glVertex2f(x + diceSize, y + diceSize);
       glVertex2f(x, y + diceSize);
    glEnd();
    
    // Draw dice border
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
       glVertex2f(x, y);
       glVertex2f(x + diceSize, y);
       glVertex2f(x + diceSize, y + diceSize);
       glVertex2f(x, y + diceSize);
    glEnd();
    
    // Draw dots
    glColor3f(0.0f, 0.0f, 0.0f);
    float dotRadius = 4.0f;
    float step = diceSize / 4.0f;
    
    auto drawDot = [x, y, step, dotRadius](float dx, float dy) {
        float dotX = x + dx * step;
        float dotY = y + dy * step;
        int segments = 10;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(dotX, dotY);
        for (int i = 0; i <= segments; i++) {
            float angle = i * 2.0f * 3.14159f / segments;
            glVertex2f(dotX + cosf(angle) * dotRadius, dotY + sinf(angle) * dotRadius);
        }
        glEnd();
    };
    
    // Draw dots based on dice value
    switch (diceRoll) {
        case 1:
            drawDot(2, 2); // Center
            break;
        case 2:
            drawDot(1, 1); // Top-left
            drawDot(3, 3); // Bottom-right
            break;
        case 3:
            drawDot(1, 1); // Top-left
            drawDot(2, 2); // Center
            drawDot(3, 3); // Bottom-right
            break;
        case 4:
            drawDot(1, 1); // Top-left
            drawDot(3, 1); // Top-right
            drawDot(1, 3); // Bottom-left
            drawDot(3, 3); // Bottom-right
            break;
        case 5:
            drawDot(1, 1); // Top-left
            drawDot(3, 1); // Top-right
            drawDot(2, 2); // Center
            drawDot(1, 3); // Bottom-left
            drawDot(3, 3); // Bottom-right
            break;
        case 6:
            drawDot(1, 1); // Top-left
            drawDot(3, 1); // Top-right
            drawDot(1, 2); // Middle-left
            drawDot(3, 2); // Middle-right
            drawDot(1, 3); // Bottom-left
            drawDot(3, 3); // Bottom-right
            break;
    }
}

// Draw player turn indicator
void drawPlayerTurn() {
    // Draw text to indicate current player
    glColor3f(0.0f, 0.0f, 0.0f);
    std::string playerText = "Player " + std::to_string(currentPlayer + 1) + "'s turn";
    glRasterPos2f(10, WIN_H - 20);
    for (char ch : playerText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch);
    }
    
    // Draw dice roll value if there is one
    if (diceRoll > 0) {
        std::string rollText = "Rolled a " + std::to_string(diceRoll);
        glRasterPos2f(10, WIN_H - 40);
        for (char ch : rollText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ch);
        }
    }
}

// Check if a player has won
bool checkWin() {
    if (playerPos[0] == 100) {
        std::cout << "Player 1 wins!" << std::endl;
        return true;
    }
    if (playerPos[1] == 100) {
        std::cout << "Player 2 wins!" << std::endl;
        return true;
    }
    return false;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBoard();
    drawJumps();
    drawToken(0);
    drawToken(1);
    drawDice();
    drawPlayerTurn();
    glutSwapBuffers();
}

// Timer function for animation
void timerCallback(int value) {
    // Move one step
    playerPos[currentPlayer]++;
    
    if (playerPos[currentPlayer] < animTarget) {
        // Schedule next step
        glutTimerFunc(200, timerCallback, 0);
    } else {
        // Landed: check snake/ladder
        int j = jumpTable[playerPos[currentPlayer]];
        if (j != 0) {
            // Add a delay before the jump
            glutTimerFunc(400, [](int) {
                playerPos[currentPlayer] = jumpTable[playerPos[currentPlayer]];
                glutPostRedisplay();
                
                // Check for win after jump
                if (!checkWin()) {
                    // Switch player after a delay
                    glutTimerFunc(600, [](int) {
                        animating = false;
                        currentPlayer = 1 - currentPlayer;
                        glutPostRedisplay();
                    }, 0);
                }
            }, 0);
        } else {
            // Check for win
            if (!checkWin()) {
                // Switch player
                animating = false;
                currentPlayer = 1 - currentPlayer;
            }
        }
    }
    
    glutPostRedisplay();
}

// Handle dice roll & start animation
void onKey(unsigned char key, int x, int y) {
    if (key == ' ' && !animating) {
        diceRoll = rand() % 6 + 1;
        int target = playerPos[currentPlayer] + diceRoll;
        
        if (target > 100) {
            // Invalid roll, switch player
            std::cout << "Invalid roll! Need exactly " << 100 - playerPos[currentPlayer] << " to win." << std::endl;
            currentPlayer = 1 - currentPlayer;
            glutPostRedisplay();
            return;
        }
        
        std::cout << "Player " << currentPlayer + 1 << " rolled a " << diceRoll << std::endl;
        
        animTarget = target;
        animating = true;
        
        // Schedule first step
        glutTimerFunc(200, timerCallback, 0);
    }
    
    // 'r' key to restart game
    if (key == 'r') {
        playerPos[0] = 0;
        playerPos[1] = 0;
        currentPlayer = 0;
        diceRoll = 0;
        animating = false;
        std::cout << "Game restarted!" << std::endl;
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(1, 1, 1, 1);
    glShadeModel(GL_FLAT);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Print instructions
    std::cout << "====== Snakes & Ladders ======" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  SPACE - Roll dice" << std::endl;
    std::cout << "  R     - Restart game" << std::endl;
    std::cout << "============================" << std::endl;
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    initJumps();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Snakes & Ladders");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(onKey);
    
    glutMainLoop();
    return 0;
}