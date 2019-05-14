#include <iostream>
#include "GameBoard.h"
using namespace std;

void GameBoard::setField(int x, int y, Field field) {
    this->board[y][x] = field;
}

GameBoard::GameBoard(int x, int y, int maxMoves) {
    this->x = x;
    this->y = y;
    this->maxMoves = maxMoves;
    this->board = new Field*[y];
    for(int i=0; i<y; ++i){
        board[i] = new Field[x];
    }
}

GameBoard::~GameBoard() {
    for(int i=0; i<y; ++i){
        delete[] board[i];
    }
    delete[] this->board;
}

Field GameBoard::getField(int x, int y) {
    return this->board[y][x];
}

void GameBoard::print() {
    cout << this->x << " " << this->y << '\n';
    cout << this->maxMoves << '\n';

    for (int y = 0; y < this->y; ++y) {
        for (int x = 0; x < this->x; ++x) {
            cout << (char) board[y][x];
        }
        cout << '\n';
    }
}
