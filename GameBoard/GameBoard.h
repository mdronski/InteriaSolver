#include <vector>
#include "../Field.h"

using namespace std;

class GameBoard {
public:
    Field **board;
    int x;
    int y;
    int maxMoves;
    void setField(int x, int y, Field field);
    Field getField(int x, int y);
    void print();

    GameBoard(int x, int y, int maxMoves);

    virtual ~GameBoard();
};
