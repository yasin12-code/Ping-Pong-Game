#include<iostream>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<time.h>

using namespace std;

char paddleSymbol[] = "บบบบบ";
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void printBorder() {
    for (int i = 2; i <= 79; i++) {
        gotoxy(i, 1);
        cout << '-';
        gotoxy(i, 25);
        cout << '-';
    }

    for (int i = 1; i <= 25; i++) {
        gotoxy(2, i);
        cout << '|';
        gotoxy(79, i);
        cout << '|';
    }
}

void setGameLayout() {
    system("cls");
    printBorder();
    gotoxy(4, 3);
    cout << "SCORE : 0";
    gotoxy(50, 3);
    cout << "Press Esc key to quit game";

    for (int i = 3; i <= 78; i++) {
        gotoxy(i, 4);
        cout << '-';
    }

    for (int i = 0; i <= strlen(paddleSymbol); i++) {
        gotoxy(5, 5 + i);
        cout << paddleSymbol[i];
        gotoxy(76, 5 + i);
        cout << paddleSymbol[i];
    }
}

void setCursorVisibility(bool visible, DWORD size = 20) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = visible;
    cursorInfo.dwSize = size;
    SetConsoleCursorInfo(console, &cursorInfo);
}

void displayMenu();

void gameplay() {
    setGameLayout();

    int score = 0;
    int isPaused = 0, isFirstMove = 1;
    int playerPaddlePos = 5, opponentPaddlePos = 5;
    int ballX = 73, ballY = 6 + rand() % 15;
    int ballDirection = rand() % 2;
    int prevBallX, prevBallY;
    int gameActive = 1, gameOver = 1;
    int rightLowerDiagonal = 0, rightUpperDiagonal = 0, leftUpperDiagonal = 0, leftLowerDiagonal = 0;

    while (1) {
        if (gameActive == 1) {
            while (!_kbhit() && gameOver) {
                prevBallX = ballX;
                prevBallY = ballY;

                gotoxy(ballX, ballY);
                cout << "O";
                Sleep(50);
                gotoxy(ballX, ballY);
                cout << " ";

                if (isFirstMove == 1) {
                    isFirstMove = 0;
                    if (ballDirection == 0) {
                        ballX--;
                        ballY++;
                    } else {
                        ballX--;
                        ballY--;
                    }
                }

                if (rightLowerDiagonal) {
                    ballX--;
                    ballY++;
                }

                if (rightUpperDiagonal) {
                    ballX--;
                    ballY--;
                }

                if (leftUpperDiagonal) {
                    ballX++;
                    ballY--;
                }

                if (leftLowerDiagonal) {
                    ballX++;
                    ballY++;
                }

                if (ballX < prevBallX && ballY > prevBallY) rightLowerDiagonal = 1;
                if (ballX < prevBallX && ballY < prevBallY) rightUpperDiagonal = 1;

                if (ballY == 5 && rightUpperDiagonal) {
                    rightLowerDiagonal = 1;
                    rightUpperDiagonal = 0;
                }

                if (ballY == 24 && rightLowerDiagonal) {
                    rightUpperDiagonal = 1;
                    rightLowerDiagonal = 0;
                }

                if (ballX == 6 && rightUpperDiagonal) {
                    leftUpperDiagonal = 1;
                    rightUpperDiagonal = 0;
                }

                if (ballX == 6 && rightLowerDiagonal) {
                    leftLowerDiagonal = 1;
                    rightLowerDiagonal = 0;
                }

                if (ballY == 5 && leftUpperDiagonal) {
                    leftLowerDiagonal = 1;
                    leftUpperDiagonal = 0;
                }

                if (ballY == 24 && leftLowerDiagonal) {
                    leftUpperDiagonal = 1;
                    leftLowerDiagonal = 0;
                }

                if (ballX == 75 && leftLowerDiagonal) {
                    rightLowerDiagonal = 1;
                    leftLowerDiagonal = 0;
                }

                if (ballX == 75 && leftUpperDiagonal) {
                    rightUpperDiagonal = 1;
                    leftUpperDiagonal = 0;
                }

                if (leftUpperDiagonal || leftLowerDiagonal) {
                    if (ballY >= 6 && ballY <= 22) {
                        if (ballY > opponentPaddlePos + strlen(paddleSymbol) - 3) {
                            gotoxy(76, opponentPaddlePos + strlen(paddleSymbol));
                            cout << "บ";
                            gotoxy(76, opponentPaddlePos);
                            if (opponentPaddlePos != 4) cout << ' ';
                            opponentPaddlePos++;
                        }
                        if (ballY < opponentPaddlePos + strlen(paddleSymbol) - 3) {
                            gotoxy(76, opponentPaddlePos);
                            cout << "บ";
                            gotoxy(76, opponentPaddlePos + strlen(paddleSymbol));
                            if (opponentPaddlePos + strlen(paddleSymbol) != 25) cout << ' ';
                            opponentPaddlePos--;
                        }
                    }
                }

                if (ballX == 6 && (ballY < playerPaddlePos || ballY > playerPaddlePos + strlen(paddleSymbol) - 1)) {
                    gotoxy(ballX, ballY);
                    cout << "YOU LOSE! Press 'r' or ENTER to play again!";
                    gameOver = 0;
                    break;
                }

                if (ballX == 6 && gameOver == 1) {
                    gotoxy(4, 3);
                    cout << "SCORE : " << ++score;
                }
            }
        }

        char keyPress = _getch();

        if (keyPress == ' ' && gameOver == 1) {
            isPaused = 1;
            gameActive = 0;
            gotoxy(22, 12);
            cout << "GAME PAUSED! PRESS ENTER to continue!";
        }

        if (keyPress == 13 && isPaused && gameOver == 1) {
            isPaused = 0;
            gameActive = 1;
            gotoxy(22, 12);
            cout << "                                       ";
        }

        if ((keyPress == 'r' || keyPress == 'R' || keyPress == 13) && gameOver == 0) {
            gameOver = 1;
            gameplay();
            break;
        }

        if ((keyPress == 's' || keyPress == 'S' || keyPress == 80) && playerPaddlePos <= 19 && gameOver) {
            gotoxy(5, playerPaddlePos + strlen(paddleSymbol));
            cout << "บ";
            gotoxy(5, playerPaddlePos);
            cout << ' ';
            playerPaddlePos++;
        }

        if ((keyPress == 'w' || keyPress == 'W' || keyPress == 72) && playerPaddlePos >= 6 && gameOver) {
            gotoxy(5, playerPaddlePos - 1);
            cout << "บ";
            gotoxy(5, playerPaddlePos + 4);
            cout << ' ';
            playerPaddlePos--;
        }

        if (keyPress == 27) {
            displayMenu();
            break;
        }
    }
}

void displayMenu() {
    system("cls");

    gotoxy(34, 4);
    cout << "PING PONG";

    for (int i = 8; i <= 73; i++) {
        gotoxy(i, 6);
        cout << '-';
    }

	gotoxy(31, 8);
    cout << "Enter Option from Menu: ";
    gotoxy(34, 10);
    cout << "1. PLAY THE GAME";
    gotoxy(34, 12);
    cout << "2. HOW TO PLAY";
    gotoxy(34, 14);
    cout << "3. EXIT";

    char option = _getche();
    if (option == '1') gameplay();
    if (option == '2') {
        system("cls");
        printBorder();
        gotoxy(4, 3);
        cout << "INSTRUCTIONS";

        gotoxy(4, 5);
        cout << "1. Use 'W' or 'w' or UP arrow key to move paddle UP.";
        gotoxy(4, 6);
        cout << "2. Use 'S' or 's' or DOWN arrow key to move paddle DOWN.";
        gotoxy(4, 7);
        cout << "3. Press SPACE key to pause the game.";
        gotoxy(4, 8);
        cout << "4. Press ESC key to return to the main menu.";
        gotoxy(4, 10);
        cout << "Press any key to return to the main menu.";
        _getch();
        displayMenu();
    }
    if (option == '3') {
        system("cls");
        gotoxy(30, 12);
        cout << "THANK YOU FOR PLAYING!";
        Sleep(2000);
        exit(0);
    }
}

int main() {
    srand(time(NULL));
    setCursorVisibility(false);
    displayMenu();
    return 0;
}
