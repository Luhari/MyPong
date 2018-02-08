#include <iostream>
#include <stdio.h>
using namespace std;

#include "Paddle.h"
#include "Ball.h"

#include <Windows.h>

int nScreenWidth = 120;			// Console Screen Size X (columns)
int nScreenHeight = 120;	        // Console Screen Size Y (rows)

int nFieldWidth = 30;           // Field
int nFieldHeight = 25;
unsigned char *pField = nullptr;

bool InField(Ball& ball) {
    return (ball.x > 0 && ball.x < nFieldWidth - 1 && ball.y > 0 && ball.y < nFieldHeight - 1);
}

bool HasToBounce(Ball& ball, Paddle& paddle) {
    switch (ball.dir) {
        case 1:
            return (ball.x-1 == 0 || (ball.y-1 == paddle.y && ball.x-1 >= paddle.x && ball.x-1 < paddle.x+5));
            break;
        case 2:
            return (ball.y-1 == paddle.y && ball.x >= paddle.x && ball.x < paddle.x+5);
            break;
        case 3:
            return (ball.x+1 == nFieldWidth - 1 || (ball.y-1 == paddle.y && ball.x+1 >= paddle.x && ball.x+1 < paddle.x+5));
            break;
        case 4:
            return (ball.x-1 == 0 || (ball.y+1 == paddle.y && ball.x-1 >= paddle.x && ball.x-1 < paddle.x+5));
            break;
        case 5:
            return (ball.y+1 == paddle.y && ball.x >= paddle.x && ball.x < paddle.x+5);
            break;
        case 6:
            return (ball.x+1 == nFieldWidth - 1 || (ball.y+1 == paddle.y && ball.x+1 >= paddle.x && ball.x+1 < paddle.x+5));
            break;
    }
    return true;
}

void MoveBall(Ball& ball) {
    switch (ball.dir) {
        case 1:
            ball.x--;ball.y--;
            break;
        case 2:
            ball.y--;
            break;
        case 3:
            ball.x++;ball.y--;
            break;
        case 4:
            ball.x--;ball.y++;
            break;
        case 5:
            ball.y++;
            break;
        case 6:
            ball.x++;ball.y++;
            break;
    }
}

//Pre: HasToBounce = true;
//Post: If has to bounce to the wall, do the symmetrical bounce. If paddle left side bounce to left, middle up, and right to right
void NewDir(Ball& ball, Paddle& paddle) {
    switch (ball.dir) {
        case 1:
            if (ball.x-1 == 0) {
                ball.dir = 3;
            }
            else /*if (ball.y-1 == paddle.y && ball.x-1 >= paddle.x && ball.x-1 < paddle.x-5)*/ {
                // Paddle Bounce
                if (ball.x-1 == paddle.x || ball.x-1 == paddle.x+1) ball.dir = 4;
                else if (ball.x-1 == paddle.x+2) ball.dir = 5;
                else ball.dir = 6;
            }
            break;
        case 2:
            // Only Paddle Bounce
            if (ball.x == paddle.x || ball.x == paddle.x+1) ball.dir = 4;
            else if (ball.x == paddle.x+2) ball.dir = 5;
            else ball.dir = 6;
            break;
        case 3:
            if (ball.x+1 == nFieldWidth - 1) {
                ball.dir = 1;
            }
            else /*if (ball.y-1 == paddle.y && ball.x+1 >= paddle.x && ball.x+1 < paddle.x-5)*/ {
                // Paddle Bounce
                if (ball.x+1 == paddle.x || ball.x+1 == paddle.x+1) ball.dir = 4;
                else if (ball.x+1 == paddle.x+2) ball.dir = 5;
                else ball.dir = 6;
            }
            break;
        case 4:
            if (ball.x-1 == 0) {
                ball.dir = 6;
            }
            else /*if (ball.y+1 == paddle.y && ball.x-1 >= paddle.x && ball.x-1 < paddle.x-5)*/ {
                // Paddle Bounce
                if (ball.x-1 == paddle.x || ball.x-1 == paddle.x+1) ball.dir = 1;
                else if (ball.x-1 == paddle.x+2) ball.dir = 2;
                else ball.dir = 3;
            }
            break;
        case 5:
            // Only Paddle Bounce
            if (ball.x == paddle.x || ball.x == paddle.x+1) ball.dir = 1;
            else if (ball.x == paddle.x+2) ball.dir = 2;
            else ball.dir = 3;
            break;
        case 6:
            if (ball.x+1 == nFieldWidth - 1) {
                ball.dir = 4;
            }
            else /*if (ball.y+1 == paddle.y && ball.x+1 >= paddle.x && ball.x+1 < paddle.x-5)*/ {
                // Paddle Bounce
                if (ball.x+1 == paddle.x || ball.x+1 == paddle.x+1) ball.dir = 1;
                else if (ball.x+1 == paddle.x+2) ball.dir = 2;
                else ball.dir = 3;
            }
            break;
    }
}

void PrintScore(char *screen, int nScore1, int nScore2, int nHitCombo) {
    sprintf(&screen[4*nScreenWidth + nFieldWidth + 25], "# SCORE #");
    sprintf(&screen[6*nScreenWidth + nFieldWidth + 16], "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 201, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 187);
    sprintf(&screen[7*nScreenWidth + nFieldWidth + 16], "%c  PLAYER 1  %c  PLAYER 2  %c", 186, 179, 186);
    sprintf(&screen[8*nScreenWidth + nFieldWidth + 16], "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 186, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 186);
    sprintf(&screen[9*nScreenWidth + nFieldWidth + 16], "%c     %d      %c     %d      %c", 186, nScore1, 196, nScore2, 186);
    sprintf(&screen[10*nScreenWidth + nFieldWidth + 16], "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 200, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 188);

    if (nHitCombo != 0) sprintf(&screen[12*nScreenWidth + nFieldWidth + 25], "HIT COMBO x%d", nHitCombo);
    else sprintf(&screen[12*nScreenWidth + nFieldWidth + 25], "              ");
}


int main() {
    // Create Screen Buffer
    char *screen = new char [nScreenWidth*nScreenHeight];
    for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = ' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    pField = new unsigned char[nFieldWidth*nFieldHeight]; // Create play field Buffer
    for (int x = 0; x < nFieldWidth; x++) {
        for (int y = 0; y < nFieldHeight; y++) {
            pField[y*nFieldWidth + x] = (y == 0 || x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1);
            if (y == nFieldHeight/2 && x != 0 && x != nFieldWidth - 1) pField[y*nFieldWidth + x] = 2;
        }
    }

    // Variables
    bool bKey[6]; //Left Right
    Paddle paddle1 (nFieldWidth / 2 - 3, nFieldHeight - 2);
    Paddle paddle2 (nFieldWidth / 2 - 2, 1);
    Ball ball (nFieldWidth / 2, nFieldHeight / 2, 5);
    int nScore1 = 0, nScore2 = 0;
        /*  Directions
            1 2 3
            0 0 0
            4 5 6
        */
    int nSpeedCount = 0, nSpeedBall = 100;
    bool turn = 0; // If 0, turn P1, if 1 turn P2;
    bool bGameOver = false;

    //Easter egg? Kappa
    int nHitCombo = 0;

    while (!bGameOver) {
        // Timing ==============================================================
        Sleep(nSpeedBall);
        if (ball.dir != 0 && nSpeedBall != 0) {
            nSpeedCount++;
            if (nSpeedBall == nSpeedCount) {
                nSpeedCount = 0;
                nSpeedBall--;
            }
        }

        // Input ===============================================================
        for (int k = 0; k < 6; k++) { //0-> L  1->R  |  A-> L  D-> R | C-> Continue
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x25\x27\x41\x44\x43\x1B"[k]))) != 0;
        }

        // Game Logic ==========================================================
            // Paddle Movement
        paddle1.x -= (bKey[0] && paddle1.x - 1 > 0);
        paddle1.x += (bKey[1] && paddle1.x + 1 < nFieldWidth-5);
        paddle2.x -= (bKey[2] && paddle2.x - 1 > 0);
        paddle2.x += (bKey[3] && paddle2.x + 1 < nFieldWidth-5);

            // Ball Movement
        if (InField(ball)) {
            if (!HasToBounce(ball, paddle1) && !HasToBounce(ball, paddle2)) MoveBall(ball);
            else if (HasToBounce(ball, paddle1)) {
                NewDir(ball, paddle1);
                MoveBall(ball);
                if (nSpeedBall == 0 && ball.dir != 0) nHitCombo++;
            }
            else {
                NewDir(ball, paddle2);
                MoveBall(ball);
                if (nSpeedBall == 0 && ball.dir != 0) nHitCombo++;
            }
        }
        else { // Ball went through the paddles
            // Show message of game ended and if wanted to continue playing
            sprintf(&screen[27*nScreenWidth + 8], "PRESS C TO CONTINUE");
            if (ball.y == 0) {
                nScore1++;
                turn = 0; // Scores P2, so begins P1;
            }
            else {
                nScore2++;
                turn = 1;
            }

            // Reset the Ball pos and dir
            ball.x = nFieldWidth/2;
            ball.y = nFieldHeight/2;
            ball.dir = 0;
        }

        // If continue playing
        if (bKey[4] && ball.dir == 0) {
            if (!turn) ball.dir = 5;
            else ball.dir = 2;
            if (nSpeedBall == 0) {
                nHitCombo = 0;
                nSpeedBall = 20;
            }
        }

        // End game
        if (bKey[5]) bGameOver = true;

        // Display ==============================================================
            // Draw Field
        for (int x = 0; x < nFieldWidth; x++) {
            for (int y = 0; y < nFieldHeight; y++) {
                screen[(y+2)*nScreenWidth + (x+2)] = " #="[pField[y*nFieldWidth + x]];
            }
        }
            // Draw paddles
        for (int px = 0; px < 5; px++) {
                screen[(paddle1.y + 2)*nScreenWidth + (paddle1.x + px + 2)] = '1';
                screen[(paddle2.y + 2)*nScreenWidth + (paddle2.x + px + 2)] = '2';
        }
            // Draw Ball
        screen[(ball.y + 2)*nScreenWidth + (ball.x + 2)] = 'O';

            // Draw Instructions
        sprintf(&screen[2*nScreenWidth + nFieldWidth + 6],"Arrows to move Paddle 1, A D to move Paddle 2. HUE");
        //sprintf(&screen[20*nScreenWidth + nFieldWidth + 6],"DEBUG: SpeedBall: %d SpeedCount: %d", nSpeedBall,  nSpeedCount);

            // Draw Score
        PrintScore(screen, nScore1, nScore2, nHitCombo);

            // Display Frame
        screen[nScreenWidth * nScreenHeight - 1] = '\0';
    	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    int winner = 0;
    if (nScore1 > nScore2) winner = 1;
    else if (nScore2 > nScore1) winner = 2;

    // Game Over
    CloseHandle(hConsole);
    cout << "Game Over!" << endl;
    cout << "Player 1 has scored " << nScore1 << endl << "Player 2 has scored " << nScore2 << endl;

    if (nHitCombo == 0) {
        if (!winner) cout << "SO YOU'VE TIED! HA HAAAAA" << endl; //HUE
        else if (winner == 1) cout << "So Player 1 has won!!" << endl; //Kappa
        else cout << "So Player 2 has won!" << endl;
    }
    else {
        if (nHitCombo == 1) cout << "LEL, just 1 hit combo xddddddddddddddd" << endl;
        else {
            cout << "Wow... You guys really are a thing. You've done " << nHitCombo << " hits, GG WP!" << endl;
        }
    }

    system("pause");
    return 0;

}
