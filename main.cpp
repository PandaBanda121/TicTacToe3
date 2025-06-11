#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <queue>
#include <stack>

#include <Windows.h>
#pragma execution_character_set("utf-8")

#include <fcntl.h>

using namespace std;


vector<int> places = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
// player 1: 0
// player 2: 1
// │ ╲╱

queue<int> p1places = {};
queue<int> p2places = {};

string hedge = "────┼────┼────\n";
vector<string> row11 = {"    ","    ","    ","\n"};
vector<string> row12 = {"    ","    ","    ","\n"};
vector<string> row21 = {"    ","    ","    ","\n"};
vector<string> row22 = {"    ","    ","    ","\n"};
vector<string> row31 = {"    ","    ","    ","\n"};
vector<string> row32 = {"    ","    ","    ","\n"};

vector<vector<string>> rows = {row11, row12, row21, row22, row31, row32};


int AIthinkTime = 500000;



void printMainScreen() {
    cout << "\033[2J\033[1;1H";
    for(int i = 0; i < 9; i++) {
        int col = i%3; //0, 1, 2
        int row = int(i/3); // 0, 1, 2
        // cout << row << col << " ";
        // if(i%3 == 2) cout << "\n";
        
        if(places[i] == -1) {
            rows[2*row][col] =   "    ";
            rows[2*row+1][col] = "    ";
        }
        if(places[i] == 0) {
            rows[2*row][col] =   " ╲╱ ";
            rows[2*row+1][col] = " ╱╲ ";
        }
        if(places[i] == 1) {
            rows[2*row][col] =   " ╱╲ ";
            rows[2*row+1][col] = " ╲╱ ";
        }        
    }
    for(int i = 0; i < 6; i++) {
       cout << rows[i][0] << "│" << rows[i][1] << "│" << rows[i][2] << rows[i][3];
        if(i == 1 || i == 3) cout << hedge;
    }
    cout << "\n";
}

void printIntroScreen() {
    cout << "\033[2J\033[1;1H";
    cout << "I'll make a simple intro screen for this version of tic tac toe!" << endl;
    cout << "Here are the following rules:" << endl;
    cout << "Same rules, except only 3 of your pieces are allowed on the board." << endl;
    cout << "When you have 3 on the board, placing the next piece will remove the oldest one." << endl;
    cout << "Have fun!" << endl;
    cout << "Press 1 for single player" << endl;
    cout << "Press 2 for double player" << endl;
}

bool isValidMove(int move) {
    if(move>=1 && move<=9) {
        if(places[move-1] == -1) return true;
    }
    return false;
}

// 012
// 345
// 678

//simulate cpu move, if win detected, stop
//if no win, simulate player move, if loss detected, stop
//if no win/loss, simulate most optimal player move for win, then simulate most optimal cpu move to prevent loss
//damn wtf lol


// How to make the cpu?
// 1. write out all possibilities for the AI win sequences, see which moves have the highest win rate, then make the cpu move according to that
// 2. 


bool checkWin(vector<int> checkingMap) {
    for(int i = 0; i <= 2; i++) {
        if(checkingMap[3*i] == checkingMap[3*i+1] && checkingMap[3*i+1] == checkingMap[3*i+2] && checkingMap[3*i+2] != -1) return true;
        if(checkingMap[i] == checkingMap[i+3*1] && checkingMap[i+3*1] == checkingMap[i+3*2] && checkingMap[i+3*2] != -1) return true;
    }
    if(checkingMap[0] == checkingMap[4] && checkingMap[4] == checkingMap[8] && checkingMap[8] != -1) return true;
    if(checkingMap[2] == checkingMap[4] && checkingMap[4] == checkingMap[6] && checkingMap[6] != -1) return true;
    return false;
}

int AImove() {
    int optimalMove = -1;
    vector<int> potentialPlaces = {};
    vector<int> potentialAIMoves = {};
    for(int i : places) potentialPlaces.push_back(i);
    queue<int> potentialp1places = p1places;
    queue<int> potentialp2places = p2places;
    
    for(int i = 0; i < 9; i++) if(potentialPlaces[i] == -1) potentialAIMoves.push_back(i);
    for(int i : potentialAIMoves) cout << i << " ";
    for(int i : potentialAIMoves) {
        potentialPlaces[i] = 1;
        potentialp2places.push(i);
        if(potentialp2places.size() > 3) {
            places[potentialp2places.front()] = -1;
            potentialp2places.pop();
        }
        if(checkWin(potentialPlaces)) return i;
        potentialp2places = p2places;
        potentialPlaces[i] = -1;
    }
    
    for(int i : potentialAIMoves) {
        potentialPlaces[i] = 0;
        potentialp1places.push(i);
        if(potentialp1places.size() > 3) {
            places[potentialp1places.front()] = -1;
            potentialp1places.pop();
        }
        if(checkWin(potentialPlaces)) return i;
        potentialp1places = p1places;
        potentialPlaces[i] = -1;
    }

    if(optimalMove == -1) {
        bool isValid = false;
        while(!isValid) {
            optimalMove = rand()%9;
            if(places[optimalMove] == -1) isValid = true;
        }
    }
    return optimalMove;
}




int main() {
    
    cout << "Working" << endl;
    // for(int i = 0; i < 9; i++) {
    //     places[i] = 1;
    //     printMainScreen();
    //     places[i] = 2;
    //     printMainScreen();
    // }
    srand(time(0));
    printIntroScreen();
    int move = int(getch())-48; //char 1 = int 49
    while(move != 1 && move != 2) {
        // cout << move << endl;
        move = int(getch())-48;
    }
    printMainScreen();
    bool singleplayer = !(move-1);
    move = -1;
    int turn = 0;
    bool gameover = false;
    while(!gameover) {
        if(turn == 0) {
            while(!isValidMove(move)) move = int(getch())-48;
            places[move-1] = 0;
            p1places.push(move-1);
            if(p1places.size()>3) {
                places[p1places.front()] = -1;
                p1places.pop();
            }
        } else {
            if(singleplayer) {
                // AI MOVE: currently just rng
                cout << "Computer thinking..." << endl;
                usleep(AIthinkTime);
                move = AImove();
                places[move] = 1;
                p2places.push(move);
                if(p2places.size()>3) {
                    places[p2places.front()] = -1;
                    p2places.pop();
                }
            } else {
                while(!isValidMove(move)) move = int(getch())-48;
                places[move-1] = 1;
                p2places.push(move-1);
                if(p2places.size()>3) {
                    places[p2places.front()] = -1;
                    p2places.pop();
                }
            }
        }
        printMainScreen();
        turn = (turn+1)%2;
        gameover = checkWin(places);
        move = -1;
    }

    string winner = "";
    if(turn == 1) winner = "X";
    else winner = "O";
    cout << winner << " is the winner!" << endl;
    

}