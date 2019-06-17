#ifndef PROJEKT_GAMEBOARD_H
#define PROJEKT_GAMEBOARD_H

#include <vector>
#include <sstream>
#include <utility>
#include <fstream>
#include <string>
#include "../Field.h"

using namespace std;

class GameBoard {
public:
    Field **board;
    int x;
    int y;
    int maxMoves;

    GameBoard();

    void setField(int y, int x, Field field);
    Field getField(int y, int x);
    void print();

    GameBoard(int x, int y, int maxMoves);

    virtual ~GameBoard();
    friend std::istream &operator>>(std::istream &str, GameBoard &data);
};



#endif //PROJEKT_GAMEBOARD_H
