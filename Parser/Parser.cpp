#include <utility>
#include <fstream>
#include <sstream>

#include "Parser.h"

using namespace std;

Parser::Parser(string filename) {
    this->filename = move(filename);
}

Parser::Parser() = default;

GameBoard Parser::parse(string filename) {
    ifstream in(filename);
    string line;
    int x, y, maxMoves;

    getline(in, line);
    istringstream (line) >> y >> x;
    getline(in, line);
    istringstream (line) >> maxMoves;
    GameBoard board = GameBoard(x, y, maxMoves);

    int current_row = 0;
    while (getline(in, line)) {
        for (int i = 0; i < line.length(); ++i) {
            board.setField(current_row, i, (Field) line[i]);
        }
        current_row++;
    }
    board.print();
    return board;
}
