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


vector<int> map = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
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


void printMainScreen() {
    cout << "\033[2J\033[1;1H";
    for(int i = 0; i < 9; i++) {
        int col = i%3; //0, 1, 2
        int row = int(i/3); // 0, 1, 2
        // cout << row << col << " ";
        // if(i%3 == 2) cout << "\n";
        
        if(map[i] == -1) {
            rows[2*row][col] =   "    ";
            rows[2*row+1][col] = "    ";
        }
        if(map[i] == 0) {
            rows[2*row][col] =   " ╲╱ ";
            rows[2*row+1][col] = " ╱╲ ";
        }
        if(map[i] == 1) {
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
        if(map[move-1] == -1) return true;
    }
    return false;
}

// 012
// 345
// 678

bool checkWin() {
    for(int i = 0; i <= 2; i++) {
        if(map[3*i] == map[3*i+1] && map[3*i+1] == map[3*i+2] && map[3*i+2] != -1) return true;
        if(map[i] == map[i+3*1] && map[i+3*1] == map[i+3*2] && map[i+3*2] != -1) return true;
    }
    if(map[0] == map[4] && map[4] == map[8] && map[8] != -1) return true;
    if(map[2] == map[4] && map[4] == map[6] && map[6] != -1) return true;
    return false;
}

int main() {
    
    cout << "Working" << endl;
    // for(int i = 0; i < 9; i++) {
    //     map[i] = 1;
    //     printMainScreen();
    //     map[i] = 2;
    //     printMainScreen();
    // }
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
        if(singleplayer) {
            break;
        } else {
            while(!isValidMove(move)) move = int(getch())-48;
            map[move-1] = turn;
            if(turn == 0) p1places.push(move-1);
            else p2places.push(move-1);
            
            int placeTakenOff = -1;
            bool needsPop = false;
            if(p1places.size()>3) {
                needsPop = true;
                placeTakenOff = p1places.front();
                p1places.pop();
            }
            if(p2places.size()>3) {
                needsPop = true;
                placeTakenOff = p2places.front();
                p2places.pop();
            }
            if(needsPop) map[placeTakenOff] = -1;
            printMainScreen();
            turn = (turn+1)%2;
            gameover = checkWin();
            move = -1;
        }
    }
    string winner = "";
    if(turn == 1) winner = "X";
    else winner = "O";
    cout << winner << " is the winner!" << endl;
    

}