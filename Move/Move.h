//
// Created by mdronski on 13.06.19.
//

#ifndef PROJEKT_MOVE_H
#define PROJEKT_MOVE_H

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
    Move(Direction direction, Vertex *destination);
    Direction direction;
    Vertex *destination;
};


#endif //PROJEKT_MOVE_H
