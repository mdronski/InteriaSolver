//
// Created by mdronski on 13.06.19.
//

#ifndef PROJEKT_MOVE_H
#define PROJEKT_MOVE_H

#include "../Gem.h"
#include <vector>


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

    std::vector<Gem*> gems;
};


#endif //PROJEKT_MOVE_H
