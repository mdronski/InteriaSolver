#ifndef PROJEKT_VERTEX_H
#define PROJEKT_VERTEX_H


#include <vector>
#include <list>
#include "../Move/Move.h"

class Vertex {
public:
    Vertex(int y, int x);

public:
    std::vector<Move> neighbours;
    int x;
    int y;
};


#endif //PROJEKT_VERTEX_H
