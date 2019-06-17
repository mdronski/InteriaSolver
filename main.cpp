#include <iostream>
#include "Solver/Solver.h"
#include "Parser/Parser.h"
#include "Graph/Graph.h"

int main(int argc, char **argv) {
//    auto *p = new Parser();
//    GameBoard gameBoard = p->parse("/home/mdronski/IET/semestr_6/ADPTO/InteriaSolver/test2.txt");


    GameBoard gameBoard = GameBoard();
    if(argc > 1){
        ifstream in(argv[1]);
        in >> gameBoard;
    } else {
        cin >> gameBoard;
    }
    Graph graph = Graph(&gameBoard);
    Solver solver =  Solver(&graph, 200);
    solver.run();
    return 0;
}