//
// Created by mdronski on 17.06.19.
//

#ifndef PROJEKT_SOLVER_H
#define PROJEKT_SOLVER_H


#include "../Graph/Graph.h"

class Solver {
public:
    Solver(Graph *graph, int maxLength);

    Graph *graph;
    int maxLength;
    solverArgs args;

    void takeGem(Gem *g);
    void returnGem(Gem *g);
    bool isFinished();
    void run();
    std::pair<bool, vector<Move>> solve(Vertex *root, std::pair<bool, vector<Move>> solution, int length);
};




#endif //PROJEKT_SOLVER_H
