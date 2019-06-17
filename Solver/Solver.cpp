
#include "Solver.h"
#include <algorithm>
#include <queue>
#include <iostream>

Solver::Solver(Graph *graph, int maxLength) : graph(graph), maxLength(maxLength) {
    args = graph->parseForSolver();
}

bool comp(pair<Vertex *, int> a, pair<Vertex *, int> b) {
    return a.second < b.second;
}

bool anyLeft(Gem *g){
    return not g->taken;
}

std::pair<bool, vector<Move>>
Solver::solve(Vertex *root, std::pair<bool, vector<Move>> solution, int length) {
    if (length > maxLength) {
        solution.first = false;
        return solution;
    }

    if (isFinished()) {
        solution.first = true;
        return solution;
    }

    vertexParams params = args[root];
    std::unordered_map<Vertex *, vector<Move>> moves = std::get<0>(params);
    std::unordered_map<Vertex *, int> shortestPaths = std::get<1>(params);

    std::vector<std::pair<Vertex *, int>> paths(shortestPaths.begin(), shortestPaths.end());
    std::sort(paths.begin(), paths.end(), comp);

    std::vector<std::vector<bool>> visitedVertexes (graph->gameBoard->y, std::vector<bool>(graph->gameBoard->x, false));
    std::queue<Vertex *> queue;
    auto v = paths.front().first;
    visitedVertexes[v->y][v->x] = true;
    queue.push(v);

    //cout << "Pushed " << v->y << "," << v->x << "\n";

    while (not queue.empty()) {
//        std::cin.ignore();
        v = queue.front();
        queue.pop();

        //cout << "Poped " << v->y << "," << v->x << "\n";

        for (auto m : v->neighbours) {

            //cout << "Analyzing " << " " << m.start->y << "," << m.start->x << " " << (char) m.direction << " " << m.destination->y << "," << m.destination->x << "\n";

            if (not m.gems.empty()) {
                std::vector<Gem *> freeGems;
                std::copy_if(m.gems.begin(), m.gems.end(), std::back_inserter(freeGems), [](Gem *g){return not g->taken;});
                if (freeGems.empty())
                    continue;

                //cout << "\tIt has gems! " << "\n";

                for (auto g : freeGems){
                    takeGem(g);
                    //cout << "\tTaken gem at " << g->y << "," << g->x << "\n";
                }

                auto partialSolution = solve(m.destination, solution, length + 1);
                if (partialSolution.first) {
                    partialSolution.second.insert(partialSolution.second.begin(), moves[m.destination].begin(), moves[m.destination].end());

                    //cout << "\tIt belongs to solution! " << "\n";
                    return partialSolution;
                }

                //cout << "\tIt doesnt belong to solution! " << "\n";

                for (auto g : freeGems){
                    returnGem(g);
                    //cout << "\tReturned gem at " << g->y << "," << g->x << "\n";
                }
            }

        }

        length++;
        for (auto m : v->neighbours) {
            auto neighbour = m.destination;
            if (visitedVertexes[neighbour->y][neighbour->x])
                continue;
            //cout << "Pushed " << neighbour->y << "," << neighbour->x << "\n";
            queue.push(neighbour);
            visitedVertexes[neighbour->y][neighbour->x] = true;
        }
    }
    return solution;
}

void Solver::takeGem(Gem *g) {
    for (Gem *gem : this->graph->gems) {
        if (gem->y == g->y and gem->x == g->x)
            gem->taken = true;
    }
}

void Solver::returnGem(Gem *g) {
    for (Gem *gem : this->graph->gems) {
        if (gem->y == g->y and gem->x == g->x)
            gem->taken = false;
    }
}

bool Solver::isFinished() {
    for (Gem *gem : this->graph->gems) {
        if (!gem->taken)
            return false;
    }
    return true;
}

void Solver::run() {
    auto sol = solve(graph->root, make_pair(false, vector<Move>()), 0);
    if (sol.first){
        for (auto m : sol.second)
            cout << (char) m.direction;
        cout << "\n";
    }
}
