cmake_minimum_required(VERSION 3.12)
project(projekt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3")
add_executable(projekt main.cpp Field.h Parser/Parser.h Parser/Parser.cpp GameBoard/GameBoard.cpp GameBoard/GameBoard.h Graph/Graph.cpp Graph/Graph.h Direction.h Gem.cpp Gem.h Vertex/Vertex.cpp Vertex/Vertex.h Move/Move.cpp Move/Move.h Solver/Solver.cpp Solver/Solver.h)

project(allInOne)
add_executable(allInOne AllInOne.cpp)