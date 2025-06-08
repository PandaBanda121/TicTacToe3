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

    int move = -1;
    int turn = 0;
    bool gameover = false;
    while(!gameover) {
        move=rand()%9;
        if(turn == 0) {
            while(!(places[move] == -1)) move = rand()%9;
            places[move] = 0;
        } else {
            while(!(places[move] == -1)) move = rand()%9;
            places[move] = 1;
        }
        seq.push_back(move);
        turn = (turn+1)%2;
        gameover = checkXWin(places);
        if(checkOWin(places)) return {};
        bool allSpacesFilled = true;
        for(int i = 0; i < 9; i++) if(places[i] == -1) allSpacesFilled = false;
        if(allSpacesFilled) return {};
    }
    return seq;

}

int main() {
    vector<vector<int>> listOfXWins = {};
    vector<vector<int>> listOfOWins = {};
    
    int randSeed = 1;
    double numberOfWins = 1e4;
    while(listOfXWins.size() < numberOfWins) {
        vector<int> seqX = simulateXwin(randSeed);
        if(!seqX.empty()) listOfXWins.push_back(seqX);
        randSeed++;
    }

    vector<vector<int>> numberOfInitialWins = {};
    vector<vector<double>> winRate = {};
    
    int batches = 10;
    cout << fixed << setprecision(ceil(log10(numberOfWins/batches)));
    for(int i = 0; i < batches; i++) numberOfInitialWins.push_back({0,0,0,0,0,0,0,0,0});

    for(int j = 0; j < batches; j++) {
        for(int i = numberOfWins * double(j)/batches; i < numberOfWins*double(j+1)/batches; i++) {
            int first = listOfXWins[i][0];
            numberOfInitialWins[j][first] = numberOfInitialWins[j][first]+1;
        }
    }
    for(int j = 0; j < batches; j++) {
        vector<double> batchWinRate = {};
        cout << "BATCH " << j << " | " << (numberOfWins/batches) << " win cases" << endl;
        
        for(int i = 0; i < 9; i++) batchWinRate.push_back(double(numberOfInitialWins[j][i])/(numberOfWins/batches));
        for(int i = 0; i < 9; i++) cout << (i+1) << ": " << batchWinRate[i] << " | ";
        cout << endl;

        double avgCornerWinRate = (batchWinRate[0]+batchWinRate[2]+batchWinRate[6]+batchWinRate[8])/4;
        double avgEdgeWinRate = (batchWinRate[1]+batchWinRate[3]+batchWinRate[5]+batchWinRate[7])/4;
        cout << "Avg Edge Win Rate: " << avgEdgeWinRate << " | Avg Corner Win Rate: " << avgCornerWinRate << " | Avg Center Win %: " << batchWinRate[4] << endl;
        cout << "Edge to Center Win Ratio: " << avgEdgeWinRate/batchWinRate[4] << " | Corner to Center Win Ratio: " << avgCornerWinRate/batchWinRate[4] << endl << endl;
        
    }

    return 0;
}