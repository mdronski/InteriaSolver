#include <iostream>
#include "GameBoard.h"
using namespace std;

void GameBoard::setField(int y, int x, Field field) {
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

Field GameBoard::getField(int y, int x) {
    return this->board[y][x];
}

void GameBoard::print() {
    cout << this->y << " " << this->x << '\n';
    cout << this->maxMoves << '\n';

    for (int y = 0; y < this->y; ++y) {
        for (int x = 0; x < this->x; ++x) {
            cout << (char) board[y][x];
        }
        cout << '\n';
    }
}

std::istream &operator>>(std::istream &str, GameBoard &data) {
    string line;

    getline(str, line);
    istringstream (line) >> data.y >> data.x;
    getline(str, line);
    istringstream (line) >> data.maxMoves;

    data.board = new Field *[data.y];

    for (int y = 0; y < data.y; ++y) {
        data.board[y] = new Field[data.x];
        getline(str, line);
        for (int x = 0; x < data.x; ++x) {
            data.setField(y, x, (Field) line[x]);
        }
    }

    return str;
}

GameBoard::GameBoard() {}
