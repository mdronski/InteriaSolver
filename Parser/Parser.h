#include <iostream>
#include <string>
#include "../GameBoard/GameBoard.h"

using namespace std;


class Parser {
private:
    string filename;
public:
    Parser(string filename);
    Parser();
    GameBoard parse(string filename);
};


