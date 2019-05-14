cmake_minimum_required(VERSION 3.12)
project(projekt)

set(CMAKE_CXX_STANDARD 11)

add_executable(projekt main.cpp Field.h Parser/Parser.cpp Parser/Parser.h Parser/Parser.cpp Parser/Parser.h Includes.h GameBoard/GameBoard.cpp GameBoard/GameBoard.h)