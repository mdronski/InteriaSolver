//
// Created by mdronski on 13.06.19.
//

#include "Move.h"

//Move::Move(Direction direction, Vertex *destination) : direction(direction), destination(destination) {}

//Move::Move(Direction direction, Vertex *destination, Gem *gem) : direction(direction), destination(destination),
//                                                                 gem(gem) {}

//Move::Move(Direction direction, Vertex *destination, const std::vector<Gem *> &gems) : direction(direction),
//                                                                                       destination(destination),
//                                                                                       gems(gems) {}


Move::Move(Direction direction, Vertex *start, Vertex *destination, const std::vector<Gem *> &gems) : direction(
        direction), start(start), destination(destination), gems(gems) {}
