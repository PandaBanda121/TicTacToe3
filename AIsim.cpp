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
#include <map>

#include <Windows.h>
#pragma execution_character_set("utf-8")

#include <fcntl.h>

using namespace std;

map<vector<int>, int> listOfGames = {};

vector<vector<int>> initialMoveWinRates = {};



bool checkGame(vector<int> checkingMap) {
    for(int i = 0; i <= 2; i++) {
        if(checkingMap[3*i] == checkingMap[3*i+1] && checkingMap[3*i+1] == checkingMap[3*i+2] && checkingMap[3*i+2] != -1) return true;
        if(checkingMap[i] == checkingMap[i+3*1] && checkingMap[i+3*1] == checkingMap[i+3*2] && checkingMap[i+3*2] != -1) return true;
    }
    if(checkingMap[0] == checkingMap[4] && checkingMap[4] == checkingMap[8] && checkingMap[8] != -1) return true;
    if(checkingMap[2] == checkingMap[4] && checkingMap[4] == checkingMap[6] && checkingMap[6] != -1) return true;
    return false;
}

void simulateGame(int randomSeed) {
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
        gameover = checkGame(places);
        if(gameover) listOfGames.insert({seq, turn});

        turn = (turn+1)%2;
    }
}



int main() {
    // for(int i = 0; i < 2000; i++) {
    //     vector<int> seqX = simulateXwin(i);
    //     vector<int> seqO = simulateOwin(i);
    //     if(!seqX.empty()) listOfXWins.push_back(seqX);
    //     if(!seqO.empty()) listOfOWins.push_back(seqO);
    // }
    
    int randSeed = 1;
    double numberOfGames = 1e6;                                                          // change to 1e5 when trying higher/safer test cases
    while(listOfGames.size() < numberOfGames) {
        simulateGame(randSeed);
        randSeed++;
    }
    // cout << "finished" << endl;

    double XwinRate = 0.0;
    double OwinRate = 0.0;

    for(map<vector<int>, int>::iterator it = listOfGames.begin(); it != listOfGames.end(); it++) {
        if(it->second == 0) XwinRate = XwinRate+1;
        else if(it->second == 1) OwinRate = OwinRate+1;
    }
    cout << "Number of games: " << numberOfGames << " | X wins: " << XwinRate << " | O wins: " << OwinRate << endl;
    
    vector<vector<int>> twoMoves = {};
    for(int i = 0; i < 9; i++) twoMoves.push_back({0,0,0,0,0,0,0,0,0});
    
    for(map<vector<int>, int>::iterator it = listOfGames.begin(); it != listOfGames.end(); it++) {
        if(it->second == 1) {
            int firstMove = it->first[0];
            int secondMove = it->first[1];
            twoMoves[firstMove][secondMove] = twoMoves[firstMove][secondMove]+1;
        }
    }

    // cout << "O\\X | ";
    // for(int i = 0; i < 9; i++) cout << setw(3) << setfill('0') << (i+1) << " | ";

    // for(int first = 0; first < 9; first++) {
    //     cout << setw(3) << setfill('0') << (first+1) << " | ";
    //     for(int second = 0; second < 9; second++) cout << setw(3) << setfill('0')<< twoMoves[first][second] << " | ";
    //     cout << endl;
    // }
    // cout << endl;


    vector<vector<vector<int>>> threeMoves = {};
    vector<vector<int>> temp2D = {};

    for(int i = 0; i < 9; i++) temp2D.push_back({0,0,0,0,0,0,0,0,0});
    for(int i = 0; i < 9; i++) threeMoves.push_back(temp2D);

    for(map<vector<int>, int>::iterator it = listOfGames.begin(); it != listOfGames.end(); it++) {
        if(it->second == 1) {
            int firstMove = it->first[0];
            int secondMove = it->first[1];
            int thirdMove = it->first[2];
            threeMoves[firstMove][secondMove][thirdMove] = threeMoves[firstMove][secondMove][thirdMove]+1;
        }
    }


    for(int first = 0; first < 9; first++) {
        cout << "X: " << (first+1) << endl;
        cout << "O\\X  | ";
        for(int i = 0; i < 9; i++) cout << setw(4) << setfill('0') << (i+1) << " | ";
        cout << endl;
        for(int second = 0; second < 9; second++) {
            cout << setw(4) << setfill('0') << (second+1) << " | ";
            for(int third = 0; third < 9; third++) cout << setw(4) << setfill('0')<< threeMoves[first][second][third] << " | ";
            cout << endl;
        }
    }
    int maxWin = 0;
    int maxI=0, maxJ=0, maxK=0;
    int minWin = numberOfGames;
    int minI=0, minJ=0, minK=0;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            for(int k = 0; k < 9; k++) {
                int current = threeMoves[i][j][k];
                if(current != 0) {
                    if(current > maxWin) {
                        maxI = i;
                        maxJ = j;
                        maxK = k;
                        maxWin = current;
                    }
                    if(current < minWin) {
                        minWin = current;
                        minI = i;
                        minJ = j;
                        minK = k;
                    }
                }
            }
        }
    }
    cout << "Max win rate: " << maxWin << " at moves " << maxI << " " << maxJ << " " << maxK << endl;
    cout << "Min win rate: " << minWin << " at moves " << minI << " " << minJ << " " << minK << endl;
    



    // for(int i = 0; i < 9; i++) {
    //     vector<int> temp = {};
    //     for(int j = 0; j < 9; j++) temp.push_back(0);
    //     initialMoveWinRates.push_back(temp);
    // }

    /*
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
    */

    // for(int i = 0; i < numberOfGames; i++) {
    //     int first = listOfXWins[i][0];
    //     numberOfInitialWins[first] = numberOfInitialWins[first]+1;
    // }
    // for(int i = 0; i < 9; i++) winRate.push_back(numberOfInitialWins[i]/numberOfGames);

    
    // double edgeWinRate = (winRate[1]+winRate[3]+winRate[5]+winRate[7]);
    // double cornerWinRate = (winRate[0]+winRate[2]+winRate[6]+winRate[8]);
    // cout << edgeWinRate << " " << cornerWinRate << " " << (4*winRate[4]);

    // for(int i = 0; i < 9; i++) cout << (i+1) << ": " << winRate[i] << endl;

    // cout << listOfXWins.size() << " " << randSeed << endl;

    return 0;
}