#include <iostream>
#include <algorithm>
#include "Graph.h"

static Direction directions[8] {
        UP,
        UP_RIGHT,
        RIGHT,
        DOWN_RIGHT,
        DOWN,
        DOWN_LEFT,
        LEFT,
        UP_LEFT
};

Graph::Graph(GameBoard *gameBoard) {
    this->gameBoard = gameBoard;
    visitedVertexes = new bool*[gameBoard->y];
    vertexes = new Vertex**[gameBoard->y];

    for (int y = 1; y < gameBoard->y - 1; ++y) {
        visitedVertexes[y] = new bool[gameBoard->x];
        vertexes[y] = new Vertex*[gameBoard->x];
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            visitedVertexes[y][x] = false;
            vertexes[y][x] = new Vertex(y, x);

            if(gameBoard->getField(y, x) == VEHICLE){
                this->root = vertexes[y][x];
            }
            if(gameBoard->getField(y, x) == GEM){
                this->gems.emplace_back(Gem(y, x));
            }


        }
    }

    addNeighbours(root);

    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            visitedVertexes[y][x] = false;
        }
    }

    printGraph(root);

}

bool Graph::isMineOrStone(Field field) {
    return field == MINE || field == STONE_BLOCK;
}

bool Graph::isRecessOrNextIsStone(Field field, Field next) {
    return field == RECESS || next == STONE_BLOCK;
}

bool Graph::isFreeOrGem(Field field) {
    return field == FREE_SPACE || field == GEM;
}

void Graph::printAdjacencyMatrix() {
    for (int x = 0; x < gameBoard->x * gameBoard->y; ++x) {
        for (int y = 0; y < gameBoard->x * gameBoard->y; ++y) {
            string s = adjacencyMatrix[x][y] ? "1 " : "0 ";
            std::cout << s;
        }
        std::cout << "\n";
    }
}

void Graph::changeDirecion(int &y, int &x, Direction direction) {
    switch (direction) {

        case UP:
            y--;
            break;
        case UP_RIGHT:
            y--;
            x++;
            break;
        case RIGHT:
            x++;
            break;
        case DOWN_RIGHT:
            y++;
            x++;
            break;
        case DOWN:
            y++;
            break;
        case DOWN_LEFT:
            y++;
            x--;
            break;
        case LEFT:
            x--;
            break;
        case UP_LEFT:
            y--;
            x--;
            break;
    }

}

bool Graph::canGo(Field field, Field nextField) {
    return (field == FREE_SPACE || field == GEM || field == RECESS)
            && (nextField == FREE_SPACE || field == GEM );
}

void Graph::addNeighbours(Vertex *v) {
    visitedVertexes[v->y][v->x] = true;
    for (Direction dir : directions){
        coverDirection(v, dir);
    }
}

void Graph::coverDirection(Vertex *v, Direction dir) {
    int dy = 0;
    int dx = 0;

//    First iteration
    changeDirecion(dy, dx, dir);
    int act_y = v->y+dy;
    int act_x = v->x+dx;

    if (gameBoard->getField(act_y, act_x) == MINE || gameBoard->getField(act_y, act_x) == STONE_BLOCK)
        return;

    if (gameBoard->getField(act_y, act_x) == RECESS){
        v->neighbours.emplace_back(Move(dir, vertexes[act_y][act_x]));
        if (!visitedVertexes[act_y][act_x]){
            addNeighbours(vertexes[act_y][act_x]);
        }
        return;
    }

    int prev_y;
    int prev_x;
    while (true) {
        prev_x = act_x;
        prev_y = act_y;
        changeDirecion(dy, dx, dir);
        act_y = v->y+dy;
        act_x = v->x+dx;


        if (gameBoard->getField(act_y, act_x) == MINE)
            return;

        if (gameBoard->getField(act_y, act_x) == STONE_BLOCK){
            v->neighbours.emplace_back(Move(dir,vertexes[prev_y][prev_x]));
            if (!visitedVertexes[prev_y][prev_x]){
                addNeighbours(vertexes[prev_y][prev_x]);
            }
            return;
        }

        if (gameBoard->getField(act_y, act_x) == RECESS){
            v->neighbours.emplace_back(Move(dir,vertexes[act_y][act_x]));
            if (!visitedVertexes[act_y][act_x]){
                addNeighbours(vertexes[act_y][act_x]);
            }
            return;
        }
    }

}

void Graph::printGraph(Vertex *vertex) {
    if (!visitedVertexes[vertex->y][vertex->x])
        visitedVertexes[vertex->y][vertex->x] = true;
    else
        return;

//    for (int y = 1; y < gameBoard->y - 1; ++y) {
//        for (int x = 1; x < gameBoard->x - 1; ++x) {
//            cout << vertexes[y][x]->y << " " << vertexes[y][x]->x << " : ";
//            for (Move m : vertexes[y][x]->neighbours){
//                cout << m.destination->y << "," << m.destination->x << " ";
//            }
//            cout << "\n";
//        }
//    }


    cout << vertex->y << " " << vertex->x << " : ";

    for (Move m : vertex->neighbours){
        cout << m.destination->y << "," << m.destination->x << " ";
    }
    cout << "\n";
    for (Move m: vertex->neighbours){
        printGraph(m.destination);
    }
}
