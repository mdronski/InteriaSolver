#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <sstream>
#include <utility>
#include <fstream>
#include <string>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

enum Field {
    STONE_BLOCK = '#',
    GEM = '+',
    MINE = '*',
    RECESS = 'O',
    VEHICLE = '.',
    FREE_SPACE = ' '
};


class GameBoard {
public:
    Field **board;
    int x;
    int y;
    int maxMoves;

    GameBoard();

    void setField(int y, int x, Field field);

    Field getField(int y, int x);

    void print();

    GameBoard(int x, int y, int maxMoves);

    virtual ~GameBoard();

    friend std::istream &operator>>(std::istream &str, GameBoard &data);
};

void GameBoard::setField(int y, int x, Field field) {
    this->board[y][x] = field;
}

GameBoard::GameBoard() {}

std::istream &operator>>(std::istream &str, GameBoard &data) {
    string line;

    getline(str, line);
    istringstream (line) >> data.y >> data.x;
    getline(str, line);
    istringstream (line) >> data.maxMoves;

    data.board = new Field *[data.y];

    for (int y = 0; y < data.y; ++y) {
        data.board[y] = new Field[data.x];
        getline(str, line);
        for (int x = 0; x < data.x; ++x) {
            data.setField(y, x, (Field) line[x]);
        }
    }

    return str;
}

GameBoard::~GameBoard() {
    for (int i = 0; i < y; ++i) {
        delete[] board[i];
    }
    delete[] this->board;
}

Field GameBoard::getField(int y, int x) {
    return this->board[y][x];
}

void GameBoard::print() {
    cout << this->y << " " << this->x << '\n';
    cout << this->maxMoves << '\n';

    for (int y = 0; y < this->y; ++y) {
        for (int x = 0; x < this->x; ++x) {
            cout << (char) board[y][x];
        }
        cout << '\n';
    }
}


class Gem {
public:
    Gem(int y, int x);

public:
    int x;
    int y;
    bool taken = false;
};

Gem::Gem(int y, int x) : x(x), y(y) {}


enum Direction {
    UP = '0',
    UP_RIGHT = '1',
    RIGHT = '2',
    DOWN_RIGHT = '3',
    DOWN = '4',
    DOWN_LEFT = '5',
    LEFT = '6',
    UP_LEFT = '7'
};

class Vertex;

class Move {
public:
    Direction direction;
    Vertex *start;
    Vertex *destination;

    Move(Direction direction, Vertex *start, Vertex *destination, const std::vector<Gem *> &gems);

    std::vector<Gem *> gems;
};

Move::Move(Direction direction, Vertex *start, Vertex *destination, const std::vector<Gem *> &gems) : direction(
        direction), start(start), destination(destination), gems(gems) {}

class Vertex {
public:
    Vertex(int y, int x);

public:
    std::vector<Move> neighbours;
    int x;
    int y;
};

Vertex::Vertex(int y, int x) : x(x), y(y) {}


typedef std::tuple<std::unordered_map<Vertex *, vector<Move>>, std::unordered_map<Vertex *, int>> vertexParams;
typedef std::unordered_map<Vertex *, vertexParams> solverArgs;

class Graph {
private:
    void coverDirection(Vertex *v, Direction direction);

    void changeDirection(int &y, int &x, Direction direction);

    void addNeighbours(Vertex *v);


public:
    explicit Graph(GameBoard *gameBoard);

    GameBoard *gameBoard;
    vector<Gem *> gems;
    bool **visitedVertexes;
    Vertex ***vertexes;
    Vertex *root;
    std::unordered_map<Vertex *, std::unordered_map<Vertex *, vector<Move>>> paths;

    void printGraph(Vertex *vertex);

    Gem *getGem(int y, int x);

    vertexParams bellmanFord(Vertex *src);

    std::unordered_map<Vertex *, std::unordered_map<Vertex *, vector<Move>>> shorthestPaths();

    solverArgs parseForSolver();

    int getVertexNumber();

    int getEdgeNumber(Vertex *v);

    vector<Move> getAllMoves(Vertex *v);

    void printShortestPaths();
};

static Direction directions[8]{
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
    visitedVertexes = new bool *[gameBoard->y];
    vertexes = new Vertex **[gameBoard->y];

    for (int y = 1; y < gameBoard->y - 1; ++y) {
        visitedVertexes[y] = new bool[gameBoard->x];
        vertexes[y] = new Vertex *[gameBoard->x];
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            visitedVertexes[y][x] = false;
            vertexes[y][x] = new Vertex(y, x);

            if (gameBoard->getField(y, x) == VEHICLE) {
                this->root = vertexes[y][x];
            }
            if (gameBoard->getField(y, x) == GEM) {
                this->gems.emplace_back(new Gem(y, x));
            }
        }
    }

    addNeighbours(root);
//
//    for (int y = 1; y < gameBoard->y - 1; ++y) {
//        for (int x = 1; x < gameBoard->x - 1; ++x) {
//            visitedVertexes[y][x] = false;
//        }
//    }
//
//    printGraph(root);

    paths = shorthestPaths();
//    printShortestPaths();

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
    for (Direction dir : directions) {
        coverDirection(v, dir);
    }
}

void Graph::coverDirection(Vertex *v, Direction dir) {
    std::vector<Gem *> gemsOnMove;
    int dy = 0;
    int dx = 0;

    if (gameBoard->getField(v->y, v->x) == GEM) {
        gemsOnMove.emplace_back(getGem(v->y, v->x));
    }
//    First iteration
    changeDirection(dy, dx, dir);
    int act_y = v->y + dy;
    int act_x = v->x + dx;

    if (gameBoard->getField(act_y, act_x) == MINE || gameBoard->getField(act_y, act_x) == STONE_BLOCK)
        return;

    if (gameBoard->getField(act_y, act_x) == RECESS) {
        v->neighbours.emplace_back(Move(dir, v, vertexes[act_y][act_x], gemsOnMove));
        if (!visitedVertexes[act_y][act_x]) {
            addNeighbours(vertexes[act_y][act_x]);
        }
        return;
    }

    if (gameBoard->getField(act_y, act_x) == GEM) {
        gemsOnMove.emplace_back(getGem(act_y, act_x));
    }

    int prev_y;
    int prev_x;
    while (true) {
        prev_x = act_x;
        prev_y = act_y;
        changeDirection(dy, dx, dir);
        act_y = v->y + dy;
        act_x = v->x + dx;

        if (gameBoard->getField(act_y, act_x) == GEM) {
            gemsOnMove.emplace_back(getGem(act_y, act_x));
        }

        if (gameBoard->getField(act_y, act_x) == MINE)
            return;

        if (gameBoard->getField(act_y, act_x) == STONE_BLOCK) {
            v->neighbours.emplace_back(Move(dir, v, vertexes[prev_y][prev_x], gemsOnMove));
            if (!visitedVertexes[prev_y][prev_x]) {
                addNeighbours(vertexes[prev_y][prev_x]);
            }
            return;
        }

        if (gameBoard->getField(act_y, act_x) == RECESS) {
            v->neighbours.emplace_back(Move(dir, v, vertexes[act_y][act_x], gemsOnMove));
            if (!visitedVertexes[act_y][act_x]) {
                addNeighbours(vertexes[act_y][act_x]);
            }
            return;
        }
    }

}

int Graph::getVertexNumber() {
    int cnt = 0;
    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            if (visitedVertexes[y][x])
                cnt++;
        }
    }
    return cnt;
}

int Graph::getEdgeNumber(Vertex *v) {
    int cnt = 0;
    for (Move m: v->neighbours) {
        cnt += getEdgeNumber(m.destination);
    }
    return (int) v->neighbours.size() + cnt;
}

void Graph::printShortestPaths() {

    for (auto const &y : paths) {
        std::cout << y.first->y << "," << y.first->x << " : ";
        std::cout << "\n";
        std::cout << "\n";

        for (auto const &x : y.second) {
            std::cout << x.first->y << "," << x.first->x << " : ";

            for (Move m : x.second) {
                std::cout << (char) m.direction;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "\n";

    }
}


vector<Move> Graph::getAllMoves(Vertex *v) {
    if (!visitedVertexes[v->y][v->x])
        visitedVertexes[v->y][v->x] = true;
    else
        return vector<Move>();

    vector<Move> moves;
    moves.insert(moves.end(), v->neighbours.begin(), v->neighbours.end());

    for (Move m : v->neighbours) {
        vector<Move> neighbourMoves = getAllMoves(m.destination);
        moves.insert(moves.end(), neighbourMoves.begin(), neighbourMoves.end());
    }
    return moves;
}

std::unordered_map<Vertex *, std::unordered_map<Vertex *, vector<Move>>> Graph::shorthestPaths() {
    std::unordered_map<Vertex *, std::unordered_map<Vertex *, vector<Move>>> shortestPaths = {};
    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            if (visitedVertexes[y][x]) {
                shortestPaths[vertexes[y][x]] = std::get<0>(bellmanFord(vertexes[y][x]));
            }
        }
    }
    return shortestPaths;
}

std::tuple<std::unordered_map<Vertex *, vector<Move>>, std::unordered_map<Vertex *, int>>
Graph::bellmanFord(Vertex *src) {
    int V = getVertexNumber();
    std::unordered_map<Vertex *, int> dist = {};
    std::unordered_map<Vertex *, vector<Move>> paths = {};


    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            if (visitedVertexes[y][x]) {
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


    for (int i = 1; i <= V - 1; i++) {
        for (Move m : moves) {
            Vertex *u = m.start;
            Vertex *v = m.destination;
            int weight = 1;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                paths[v] = paths[u];
                paths[v].emplace_back(m);
            }
        }
    }


    return std::make_tuple(paths, dist);
}

void Graph::printGraph(Vertex *vertex) {
    if (!visitedVertexes[vertex->y][vertex->x])
        visitedVertexes[vertex->y][vertex->x] = true;
    else
        return;


    cout << vertex->y << "," << vertex->x << " : ";

    for (Move m : vertex->neighbours) {
        if (!m.gems.empty()) {
            for (Gem *gem : m.gems) {
                cout << "[" << gem->y << "," << gem->x << "]";
            }
        }
        cout << "(" << m.destination->y << "," << m.destination->x << ")" << " ";
    }
    cout << "\n";
    for (Move m: vertex->neighbours) {
        printGraph(m.destination);
    }
}

Gem *Graph::getGem(int y, int x) {
    for (Gem *gem : gems) {
        if (gem->x == x and gem->y == y)
            return gem;
    }
    return nullptr;
}

std::unordered_map<Vertex *, std::tuple<std::unordered_map<Vertex *, vector<Move>>, std::unordered_map<Vertex *, int>>>
Graph::parseForSolver() {
    std::unordered_map<Vertex *, std::tuple<std::unordered_map<Vertex *, vector<Move>>, std::unordered_map<Vertex *, int>>> alaMaKota = {};
    for (int y = 1; y < gameBoard->y - 1; ++y) {
        for (int x = 1; x < gameBoard->x - 1; ++x) {
            if (visitedVertexes[y][x])
                alaMaKota[vertexes[y][x]] = (bellmanFord(vertexes[y][x]));
        }
    }
    return alaMaKota;
}


class Solver {
public:
    Solver(Graph *graph);

    Graph *graph;
    solverArgs args;

    void takeGem(Gem *g);

    void returnGem(Gem *g);

    bool isFinished();

    void run();

    std::pair<bool, vector<Move>> solve(Vertex *root, int length);
};


Solver::Solver(Graph *graph) : graph(graph) {
    args = graph->parseForSolver();
}

bool comp(pair<Vertex *, int> a, pair<Vertex *, int> b) {
    return a.second < b.second;
}

bool anyLeft(Gem *g) {
    return not g->taken;
}

//std::pair<bool, vector<Move>>
//Solver::solve(Vertex *root, int length) {
//    if (length > graph->gameBoard->maxMoves) {
//        return make_pair(false, vector<Move>());
//    }
//
//    if (isFinished()) {
//        return make_pair(true, vector<Move>());
//    }
//
//    vertexParams params = args[root];
//    std::unordered_map<Vertex *, vector<Move>> moves = std::get<0>(params);
//    std::unordered_map<Vertex *, int> shortestPaths = std::get<1>(params);
//
//    std::vector<std::pair<Vertex *, int>> paths(shortestPaths.begin(), shortestPaths.end());
//    std::sort(paths.begin(), paths.end(), comp);
//
//    std::vector<std::vector<bool>> visitedVertexes(graph->gameBoard->y, std::vector<bool>(graph->gameBoard->x, false));
//    std::queue<Vertex *> queue;
//    auto v = paths.front().first;
//    visitedVertexes[v->y][v->x] = true;
//    queue.push(v);
//
//
//    //cout << "Pushed " << v->y << "," << v->x << "\n";
//
//    while (not queue.empty()) {
////        std::cin.ignore();
//        v = queue.front();
//        queue.pop();
//
//        //cout << "Poped " << v->y << "," << v->x << "\n";
//
//        for (auto m : v->neighbours) {
//
//            //cout << "Analyzing " << " " << m.start->y << "," << m.start->x << " " << (char) m.direction << " " << m.destination->y << "," << m.destination->x << "\n";
//
//            if (not m.gems.empty()) {
//                std::vector<Gem *> freeGems;
//                std::copy_if(m.gems.begin(), m.gems.end(), std::back_inserter(freeGems),
//                             [](Gem *g) { return not g->taken; });
//                if (freeGems.empty())
//                    continue;
//
//                //cout << "\tIt has gems! " << "\n";
//
//                for (auto g : freeGems) {
//                    takeGem(g);
//                    //cout << "\tTaken gem at " << g->y << "," << g->x << "\n";
//                }
//
//                auto partialSolution = solve(m.destination, length + 1);
//
////                if (partialSolution.first && partialSolution.second.empty()){
////                    cout << (char) m.direction << "\n";
////                    cout << m.start->y << " " << m.start->x  <<"\n";
////                    cout << m.destination->y << " " << m.destination->x  <<"\n";
////                }
////
////                if (partialSolution.first && partialSolution.second.size() == 1){
////                    cout << (char) m.direction << "\n";
////                    cout << m.start->y << " " << m.start->x  <<"\n";
////                    cout << m.destination->y << " " << m.destination->x  <<"\n";
////                }
//
//                if (partialSolution.first) {
////                    if (partialSolution.second.empty())
////                        partialSolution.second.emplace_back(m);
//
//
//                    partialSolution.second.insert(partialSolution.second.begin(), m);
//                    partialSolution.second.insert(partialSolution.second.begin(), moves[m.start].begin(),
//                                                  moves[m.start].end());
//
//
//
//                    //cout << "\tIt belongs to solution! " << "\n";
//                    return partialSolution;
//                }
//
//                //cout << "\tIt doesnt belong to solution! " << "\n";
//
//                for (auto g : freeGems) {
//                    returnGem(g);
//                    //cout << "\tReturned gem at " << g->y << "," << g->x << "\n";
//                }
//            }
//
//
//        }
//
//        length++;
//        for (auto m : v->neighbours) {
//            auto neighbour = m.destination;
//            if (visitedVertexes[neighbour->y][neighbour->x])
//                continue;
//            //cout << "Pushed " << neighbour->y << "," << neighbour->x << "\n";
//            queue.push(neighbour);
//            visitedVertexes[neighbour->y][neighbour->x] = true;
//        }
//    }
////    for (auto m : solution.second)
////        for (auto g : m.gems)
////        returnGem(g);
//
//    return make_pair(false, vector<Move>());
//}

bool allGemsTaken(vector<Gem *> gems){
    for (auto g : gems)
        if (not g->taken)
            return  false;
    return true;
}

bool moveVecComp(pair<Vertex *, pair<vector<Move>, vector<Gem*>>> p1, pair<Vertex *, pair<vector<Move>, vector<Gem*>>> p2){
    int pos1 = 0;
    int pos2 = 0;
    for (auto m : p1.second.first){
        if (not m.gems.empty() and not allGemsTaken(m.gems))
            break;
        pos1++;
    }

    for (auto m : p2.second.first){
        if (not m.gems.empty() and not allGemsTaken(m.gems))
            break;
        pos2++;
    }

    return pos1 < pos2;
}

vector<pair<Vertex *, pair<vector<Move>, vector<Gem*>>>> getMovesWithGems(std::unordered_map<Vertex *, vector<Move>> allMoves){
    vector<pair<Vertex *, pair<vector<Move>, vector<Gem*>>>> movesWithGems;
    for (auto kv : allMoves){
        for (Move m : kv.second){
            vector<Gem> tmpGems;
            if (not m.gems.empty() and not allGemsTaken(m.gems)){
                for(auto g : m.gems){
                    if ( not g->taken)
                        tmpGems.emplace_back(g);
//                        movesWithGems[kv.first].second.emplace_back(g);
                }
                movesWithGems.emplace_back(make_pair(kv.first, make_pair(kv.second, tmpGems)));
//                movesWithGems[kv.first].first = kv.second;
                continue;
            }
        }
    }

//    std::sort(movesWithGems.begin(), movesWithGems.end(), moveVecComp);
    return movesWithGems;
}


std::pair<bool, vector<Move>>
Solver::solve(Vertex *root, int length) {
    if (length > graph->gameBoard->maxMoves) {
        return make_pair(false, vector<Move>());
    }

    if (isFinished()) {
        return make_pair(true, vector<Move>());
    }

    vertexParams params = args[root];
    std::unordered_map<Vertex *, vector<Move>> moves = std::get<0>(params);
    map<Vertex *, pair<vector<Move>, vector<Gem*>>> movesWithGems = getMovesWithGems(moves);

    for (auto kv : movesWithGems){

        for (auto g :kv.second.second) {
            takeGem(g);
            }

        auto partialSolution = solve(kv.first, static_cast<int>(length + kv.second.first.size()));
        if (partialSolution.first){
            partialSolution.second.insert(
                    partialSolution.second.begin(), kv.second.first.begin(), kv.second.first.end());
            return partialSolution;
        }

        for (auto g :kv.second.second) {
            returnGem(g);
        }
    }
    return make_pair(false, vector<Move>());
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
    auto sol = solve(graph->root, 0);
    if (sol.first) {
        for (auto m : sol.second)
            cout << (char) m.direction;
        cout << "\n";
    } else {
//        while (true){
//
//        }
        cout << "BRAK";
    }
}


int main(int argc, char **argv) {
    GameBoard gameBoard = GameBoard();
    if (argc > 1) {
        ifstream in(argv[1]);
        in >> gameBoard;
    } else {
        cin >> gameBoard;
    }
    Graph graph = Graph(&gameBoard);
//    gameBoard.print();
    Solver solver = Solver(&graph);
    solver.run();
    return 0;
}