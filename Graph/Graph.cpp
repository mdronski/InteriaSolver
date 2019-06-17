#include <iostream>
#include <algorithm>
#include <climits>
#include <unordered_map>
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
                this->gems.emplace_back(new Gem(y, x));
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
//
//    for (int y = 1; y < gameBoard->y - 1; ++y) {
//        for (int x = 1; x < gameBoard->x - 1; ++x) {
//            visitedVertexes[y][x] = false;
//        }
//    }
//
//    vector<Move> moves = getAllMoves(root);
//    for (Move m : moves){
//        if (!m.gems.empty()){
//            for (Gem *gem : m.gems){
//                cout << "[" << gem->y << "," << gem->x << "]";
//            }
//        }
//        cout << "(" << m.destination->y << "," << m.destination->x << ")" << " ";
//    }

    paths = shorthestPaths();
    printShortestPaths();

}

void Graph::changeDirection(int &y, int &x, Direction direction) {
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

void Graph::addNeighbours(Vertex *v) {
    visitedVertexes[v->y][v->x] = true;
    for (Direction dir : directions){
        coverDirection(v, dir);
    }
}

void Graph::coverDirection(Vertex *v, Direction dir) {
    std::vector<Gem*> gemsOnMove;
    int dy = 0;
    int dx = 0;

//    First iteration
    changeDirection(dy, dx, dir);
    int act_y = v->y+dy;
    int act_x = v->x+dx;

    if (gameBoard->getField(act_y, act_x) == MINE || gameBoard->getField(act_y, act_x) == STONE_BLOCK)
        return;

    if (gameBoard->getField(act_y, act_x) == RECESS){
        v->neighbours.emplace_back(Move(dir, v, vertexes[act_y][act_x], gemsOnMove));
        if (!visitedVertexes[act_y][act_x]){
            addNeighbours(vertexes[act_y][act_x]);
        }
        return;
    }

    if (gameBoard->getField(act_y, act_x) == GEM){
        gemsOnMove.emplace_back(getGem(act_y, act_x));
    }

    int prev_y;
    int prev_x;
    while (true) {
        prev_x = act_x;
        prev_y = act_y;
        changeDirection(dy, dx, dir);
        act_y = v->y+dy;
        act_x = v->x+dx;

        if (gameBoard->getField(act_y, act_x) == GEM){
            gemsOnMove.emplace_back(getGem(act_y, act_x));
        }

        if (gameBoard->getField(act_y, act_x) == MINE)
            return;

        if (gameBoard->getField(act_y, act_x) == STONE_BLOCK){
            v->neighbours.emplace_back(Move(dir, v, vertexes[prev_y][prev_x], gemsOnMove));
            if (!visitedVertexes[prev_y][prev_x]){
                addNeighbours(vertexes[prev_y][prev_x]);
            }
            return;
        }

        if (gameBoard->getField(act_y, act_x) == RECESS){
            v->neighbours.emplace_back(Move(dir, v, vertexes[act_y][act_x], gemsOnMove));
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


    cout << vertex->y << "," << vertex->x << " : ";

    for (Move m : vertex->neighbours){
        if (!m.gems.empty()){
            for (Gem *gem : m.gems){
                cout << "[" << gem->y << "," << gem->x << "]";
            }
        }
        cout << "(" << m.destination->y << "," << m.destination->x << ")" << " ";
    }
    cout << "\n";
    for (Move m: vertex->neighbours){
        printGraph(m.destination);
    }
}

Gem *Graph::getGem(int y, int x) {
    for (Gem *gem : gems){
        if (gem->x==x and gem->y==y)
            return gem;
    }
    return nullptr;
}

std::unordered_map<Vertex*, vector<Move>> Graph::bellmanFord(Vertex *src) {
    int V = getVertexNumber();
    std::unordered_map<Vertex*, int> dist = {};
    std::unordered_map<Vertex*, vector<Move>> paths = {};
//    int dist[V];

    // Step 1: Initialize distances from src to all other vertices
    // as INFINITE
    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            if(visitedVertexes[y][x]){
                dist[vertexes[y][x]] = INT_MAX;
                paths[vertexes[y][x]] = vector<Move>();
            }
        }
    }

    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            visitedVertexes[y][x] = false;
        }
    }

    dist[src] = 0;
    vector<Move> moves = getAllMoves(root);

    // Step 2: Relax all edges |V| - 1 times. A simple shortest
    // path from src to any other vertex can have at-most |V| - 1
    // edges
    for (int i = 1; i <= V-1; i++)
    {
        for (Move m : moves)
        {
            Vertex *u = m.start;
            Vertex *v = m.destination;
            int weight = 1;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]){
                dist[v] = dist[u] + weight;
                paths[v] = paths[u];
                paths[v].emplace_back(m);
            }
        }
    }

//    for (auto const& x : dist){
//        std::cout << x.first->y
//                  << ","
//                  << x.first->x
//                  << " : "
//                  << x.second
//                  << std::endl ;
//    }
//
//    std::cout << "\n";
//    std::cout << "\n";
//    std::cout << "\n";
//
//
//    for (auto const& x : paths){
//        std::cout << x.first->y << "," << x.first->x << " : ";
//
//        for (Move m : x.second){
//            std::cout << (char) m.direction;
//        }
//        std::cout << "\n";
//
//    }

    return paths;
}

int Graph::getVertexNumber() {
    int cnt = 0;
    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            if(visitedVertexes[y][x])
                cnt++;
        }
    }
    return cnt;
}

int Graph::getEdgeNumber(Vertex *v) {
    int cnt = 0;
    for (Move m: v->neighbours){
        cnt += getEdgeNumber(m.destination);
    }
    return (int) v->neighbours.size() + cnt;
}

vector<Move> Graph::getAllMoves(Vertex *v) {
    if (!visitedVertexes[v->y][v->x])
        visitedVertexes[v->y][v->x] = true;
    else
        return vector<Move>();

    vector<Move> moves;
    moves.insert(moves.end(), v->neighbours.begin(), v->neighbours.end());

    for (Move m : v->neighbours){
        vector<Move> neighbourMoves = getAllMoves(m.destination);
        moves.insert(moves.end(), neighbourMoves.begin(), neighbourMoves.end());
    }
    return moves;
}

std::unordered_map<Vertex *, std::unordered_map<Vertex *, vector<Move>>> Graph::shorthestPaths() {
    std::unordered_map<Vertex *, std::unordered_map<Vertex *, vector<Move>>> shortestPaths = {};
    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            if(visitedVertexes[y][x])
                shortestPaths[vertexes[y][x]] = bellmanFord(vertexes[y][x]);
        }
    }
    return shortestPaths;
}

void Graph::printShortestPaths() {

    for (auto const& y : paths){
        std::cout << y.first->y << "," << y.first->x << " : ";
        std::cout << "\n";
        std::cout << "\n";

        for (auto const& x : y.second){
            std::cout << x.first->y << "," << x.first->x << " : ";

            for (Move m : x.second){
                std::cout << (char) m.direction;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "\n";

    }
}



