cmake_minimum_required(VERSION 4.0)
project(plota_mini_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(plota_mini_game main.cpp
        Backend/Games/Board.cpp
        Backend/Games/Board.h
        Backend/Games/Game.cpp
        Backend/Games/Game.h
        Backend/Games/Piece.cpp
        Backend/Games/Piece.h
        Backend/Games/Turn.cpp
        Backend/Games/Turn.h
        Backend/Games/Connect-4/Connect-4.cpp
        Backend/Games/Connect-4/Connect-4.h
        Backend/Tools/Time.cpp
        Backend/Tools/Time.h
        Backend/Games/Status.cpp
        Backend/Games/Status.h
        Backend/Games/Location.h
        Backend/Games/Othello/Othello.cpp
        Backend/Games/Othello/Othello.h
        Backend/Games/Checkers/Checkers.cpp
        Backend/Games/Checkers/Checkers.h)
