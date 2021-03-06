//
//  9_14.cpp
//  Programming Abstractions C++
//
//  Created by Ben Mills on 8/22/15.
//
//  MiniMax Nim with 3 rows of coins.

#include <iostream>
#include <string>
#include "simpio.h"
#include "error.h"
#include "strlib.h"
#include "console.h"
#include "vector.h"

using namespace std;

/* Constants */
const int ROW_0 = 3;
const int ROW_1 = 4;
const int ROW_2 = 5;
const int N_COINS = ROW_0 + ROW_1 + ROW_2;
const int WINNING_POSITION = 100;
const int LOSING_POSITION = -WINNING_POSITION;
const int MAX_DEPTH = 5;



/* Enumerators */
enum Player { HUMAN, COMPUTER};

Player opponent(Player player) {
    return (player == HUMAN) ? COMPUTER : HUMAN;
}

const Player STARTING_PLAYER = HUMAN;

/* Structures */
struct Move {
    int nRow;
    int nTaken;
};

/* Class */
class Nim {
    
public:
    void play() {
        initGame();
        while (!gameIsOver()) {
            displayGame();
            if (getCurrentPLayer() == HUMAN) {
                makeMove(getUserMove());
            } else {
                Move move = getComputerMove();
                displayMove(move);
                makeMove(move);
            }
            switchTurn();
        }
        announceResult();
    }
    
    void printInsructions() {
        cout << "Welcome to the game of Nim!" << endl;
        cout << "In this game, we will start with 3 rows of coins" << endl;
        cout << "on the table.  On each turn, you" << endl;
        cout << "and I will alternately take coins from one row at a time." << endl;
        cout << "The player who takes the last coin loses." << endl << endl;
    }
    
private:
    
    void initGame() {
        nCoins = N_COINS;
        whoseTurn = STARTING_PLAYER;
        coinRows += row0, row1, row2;
        for (int i = 0; i < ROW_0; i++) {
            coinRows[0].add(true);
        }
        for (int i = 0; i < ROW_1; i++) {
            coinRows[1].add(true);
        }
        for (int i = 0; i < ROW_2; i++) {
            coinRows[2].add(true);
        }
    }
    
    bool gameIsOver() {
        return (nCoins <= 1);
    }
    
    void displayGame() {
        cout << "The rows are as follows: " << endl;
        cout << "Row 0: " << coinRows[0] << endl;
        cout << "Row 1: " << coinRows[1] << endl;
        cout << "Row 2: " << coinRows[2] << endl;
        
    }
    
    Player getCurrentPLayer() {
        return whoseTurn;
    }
    
    Move getComputerMove() {
        return findBestMove();
    }
    
    void displayMove(Move move) {
        cout << "I'll take " << move.nTaken << " from Row "<< move.nRow << "." << endl;
    }
    
    void makeMove(Move move) {
        nCoins -= move.nTaken;
        for (int i = 0; i < move.nTaken; i++) {
            coinRows[move.nRow].remove(0);
        }
    }
    
    void switchTurn() {
        whoseTurn = opponent(whoseTurn);
    }
    
    Move findBestMove() {
        int rating;
        return findBestMove(0, rating);
    }
    
    Move findBestMove(int depth, int &rating) {
        Vector<Move> moveList;
        Move bestMove;
        int minRating = WINNING_POSITION + 1;
        generateMoveList(moveList);
        if (moveList.isEmpty()) error("No moves available");
        for (Move move: moveList) {
            makeMove(move);
            int moveRating = evaluatePosition(depth + 1);
            if (moveRating < minRating) {
                bestMove = move;
                minRating = moveRating;
            }
            retractMove(move);
        }
        rating = -minRating;
        return bestMove;
    }
    
    int evaluatePosition(int depth) {
        if (gameIsOver() || depth >= MAX_DEPTH) {
            return evaluateStaticPosition();
        }
        int rating;
        findBestMove(depth, rating);
        return rating;
    }

    Move getUserMove() {
        Move move;
        while (true) {
            move.nRow = getInteger("Which # Row: ");
            move.nTaken = getInteger("How many would you like: ");
            int limit = coinRows[move.nRow].size();
            if (move.nTaken > 0 && move.nTaken <= limit) return move;
            cout << "There are only " << limit << " coins left in Row" << move.nRow << endl;

        }
    }
    
    void generateMoveList(Vector<Move> &moveList) {
        Move move;
        for (int i = 0; i < coinRows.size(); i++) {
            for (int j = 1; j <= coinRows[i].size(); j++) {
                move.nRow = i;
                move.nTaken = j;
                moveList.add(move);
            }
        }
    }
    
    void retractMove(Move move) {
        nCoins += move.nTaken;
        for (int i = 0; i < move.nTaken; i++) {
            coinRows[move.nRow].add(true);
        }
    }
    
    int evaluateStaticPosition() {
        switch (nCoins) {
            case 1:
                return LOSING_POSITION;
                break;
            case 2:
                return WINNING_POSITION;
                break;
            case 3:
                return 50;
                break;
            case 4:
                return 25;
                break;
            case 5:
                return LOSING_POSITION;
                break;
            default:
                return 0;
                break;
        }
    }
    
    void announceResult() {
        if (nCoins == 0) {
            cout << "You took the last coin.  You lose." << endl;
        } else {
            cout << "There is only one coin left." << endl;
            if (whoseTurn == HUMAN) {
                cout << "I win." << endl;
            } else {
                cout << "I lose." << endl;
            }
        }
    }

    
/* Instance Variables */
    int nCoins;
    Vector<bool> row0;
    Vector<bool> row1;
    Vector<bool> row2;
    Vector<Vector<bool> > coinRows;
    Player whoseTurn;
    
};

/* Main Program */
int main() {
    Nim game;
    game.printInsructions();
    game.play();
    return 0;
}
               
