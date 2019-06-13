#ifndef PROJEKT_GRAPH_H
#define PROJEKT_GRAPH_H



#include <list>
#include "../GameBoard/GameBoard.h"
#include "../Direction.h"
#include "../cmake-build-debug/Gem.h"
#include "../Vertex/Vertex.h"

using namespace std;


class Graph {
private:
    bool isMineOrStone(Field field);
    bool isRecessOrNextIsStone(Field field, Field next);
    bool isFreeOrGem(Field field);
    bool canGo(Field field, Field nextField);
    void coverDirection(Vertex *v, Direction direction);
    void printAdjacencyMatrix();
    void changeDirecion(int &y, int &x, Direction direction);

    void addNeighbours(Vertex *v);

public:
    explicit Graph(GameBoard *gameBoard);
    GameBoard *gameBoard;
    vector<Gem> gems;
    bool **visitedVertexes;
    Vertex ***vertexes;
    Vertex *root;
    bool **adjacencyMatrix;
    void printGraph(Vertex *vertex);
};

#endif //PROJEKT_GRAPH_H
