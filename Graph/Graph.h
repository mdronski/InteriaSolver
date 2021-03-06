#ifndef PROJEKT_GRAPH_H
#define PROJEKT_GRAPH_H



#include <list>
#include "../GameBoard/GameBoard.h"
#include "../Direction.h"
#include "../Gem.h"
#include "../Vertex/Vertex.h"
#include <unordered_map>

typedef std::tuple<std::unordered_map<Vertex *, vector<Move>>, std::unordered_map<Vertex *, int>> vertexParams;
typedef std::unordered_map<Vertex *, vertexParams> solverArgs;

using namespace std;


class Graph {
private:
    void coverDirection(Vertex *v, Direction direction);
    void changeDirection(int &y, int &x, Direction direction);
    void addNeighbours(Vertex *v);


public:
    explicit Graph(GameBoard *gameBoard);
    GameBoard *gameBoard;
    vector<Gem*> gems;
    bool **visitedVertexes;
    Vertex ***vertexes;
    Vertex *root;
    std::unordered_map<Vertex*, std::unordered_map<Vertex*, vector<Move>>> paths;
    void printGraph(Vertex *vertex);
    Gem *getGem(int y, int x);
    vertexParams bellmanFord(Vertex *src);
    std::unordered_map<Vertex*, std::unordered_map<Vertex*, vector<Move>>> shorthestPaths();
    solverArgs parseForSolver();
    int getVertexNumber();
    int getEdgeNumber(Vertex *v);
    vector<Move> getAllMoves(Vertex *v);
    void printShortestPaths();
};

#endif //PROJEKT_GRAPH_H
