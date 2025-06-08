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


bool checkXWin(vector<int> checkingMap) {
    for(int i = 0; i <= 2; i++) {
        if(checkingMap[3*i] == checkingMap[3*i+1] && checkingMap[3*i+1] == checkingMap[3*i+2] && checkingMap[3*i+2] == 0) return true;
        if(checkingMap[i] == checkingMap[i+3*1] && checkingMap[i+3*1] == checkingMap[i+3*2] && checkingMap[i+3*2] == 0) return true;
    }
    if(checkingMap[0] == checkingMap[4] && checkingMap[4] == checkingMap[8] && checkingMap[8] == 0) return true;
    if(checkingMap[2] == checkingMap[4] && checkingMap[4] == checkingMap[6] && checkingMap[6] == 0) return true;
    return false;
}

bool checkOWin(vector<int> checkingMap) {
    for(int i = 0; i <= 2; i++) {
        if(checkingMap[3*i] == checkingMap[3*i+1] && checkingMap[3*i+1] == checkingMap[3*i+2] && checkingMap[3*i+2] == 1) return true;
        if(checkingMap[i] == checkingMap[i+3*1] && checkingMap[i+3*1] == checkingMap[i+3*2] && checkingMap[i+3*2] == 1) return true;
    }
    if(checkingMap[0] == checkingMap[4] && checkingMap[4] == checkingMap[8] && checkingMap[8] == 1) return true;
    if(checkingMap[2] == checkingMap[4] && checkingMap[4] == checkingMap[6] && checkingMap[6] == 1) return true;
    return false;
}

vector<int> simulateXwin(int randomSeed) {
    srand(randomSeed+time(0));
    
    vector<int> seq = {};
    vector<int> places = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
    queue<int> p1places = {};
    queue<int> p2places = {};

    int move = -1;
    int turn = 0;
    bool gameover = false;
    while(!gameover) {
        if(turn == 0) {
            while(!(places[move] == -1)) move = rand()%9;
            places[move] = 0;
            p1places.push(move);
            if(p1places.size()>3) {
                places[p1places.front()] = -1;
                p1places.pop();
            }
        } else {
            while(!(places[move] == -1)) move = rand()%9;
            places[move] = 1;
            p2places.push(move);
            if(p2places.size()>3) {
                places[p2places.front()] = -1;
                p2places.pop();
            }
        }
        seq.push_back(move);
        turn = (turn+1)%2;
        gameover = checkXWin(places);
        if(checkOWin(places)) return {};
        move = -1;
    }
    return seq;

}


vector<int> simulateOwin(int randomSeed) {
    srand(randomSeed);
    
    vector<int> seq = {};
    vector<int> places = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
    queue<int> p1places = {};
    queue<int> p2places = {};

    int move = -1;
    int turn = 0;
    bool gameover = false;
    while(!gameover) {
        if(turn == 0) {
            while(!(places[move] == -1)) move = rand()%9;
            places[move] = 0;
            p1places.push(move);
            if(p1places.size()>3) {
                places[p1places.front()] = -1;
                p1places.pop();
            }
        } else {
            while(!(places[move] == -1)) move = rand()%9;
            places[move] = 1;
            p2places.push(move);
            if(p2places.size()>3) {
                places[p2places.front()] = -1;
                p2places.pop();
            }
        }
        seq.push_back(move);
        turn = (turn+1)%2;
        gameover = checkOWin(places);
        if(checkXWin(places)) return {};
        move = -1;
    }
    return seq;

}


int main() {
    // for(int i = 0; i < 2000; i++) {
    //     vector<int> seqX = simulateXwin(i);
    //     vector<int> seqO = simulateOwin(i);
    //     if(!seqX.empty()) listOfXWins.push_back(seqX);
    //     if(!seqO.empty()) listOfOWins.push_back(seqO);
    // }
    vector<vector<int>> listOfXWins = {};
    vector<vector<int>> listOfOWins = {};
    
    int randSeed = 1;
    double numberOfWins = 1e4;
    while(listOfXWins.size() < numberOfWins) {
        vector<int> seqX = simulateXwin(randSeed);
        if(!seqX.empty()) listOfXWins.push_back(seqX);
        randSeed++;
    }

    vector<int> numberOfInitialWins = {0,0,0,0,0,0,0,0,0};
    vector<double> winRate = {};
    for(int i = 0; i < numberOfWins; i++) {
        int first = listOfXWins[i][0];
        numberOfInitialWins[first] = numberOfInitialWins[first]+1;
    }
    for(int i = 0; i < 9; i++) winRate.push_back(numberOfInitialWins[i]/numberOfWins);
    
    double edgeWinRate = (winRate[1]+winRate[3]+winRate[5]+winRate[7]);
    double cornerWinRate = (winRate[0]+winRate[2]+winRate[6]+winRate[8]);
    
    cout << edgeWinRate << " " << cornerWinRate << " " << (4*winRate[4]);

    // for(int i = 0; i < 9; i++) cout << (i+1) << ": " << winRate[i] << endl;

    // cout << listOfXWins.size() << " " << randSeed << endl;

    return 0;
}