#ifndef PROJEKT_GAMEBOARD_H
#define PROJEKT_GAMEBOARD_H

#include <vector>
#include "../Field.h"

using namespace std;

class GameBoard {
public:
    Field **board;
    int x;
    int y;
    int maxMoves;
    void setField(int y, int x, Field field);
    Field getField(int y, int x);
    void print();

    GameBoard(int x, int y, int maxMoves);

    virtual ~GameBoard();
};



#endif //PROJEKT_GAMEBOARD_H
