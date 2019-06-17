#include <iostream>
#include "Parser/Parser.h"
#include "Graph/Graph.h"

int main() {
    auto *p = new Parser();
    GameBoard gameBoard = p->parse("/home/mdronski/IET/semestr_6/ADPTO/InteriaSolver/test.txt");
    Graph graph = Graph(&gameBoard);
    return 0;
}