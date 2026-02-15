#pragma once


#include <filesystem>
#include <format>

#include "Board.h"
#include "Turn.h"
#include "Status.h"
#include <string>
#include <vector>


class Game {
protected:
     Board src;
     int id;
public:
     virtual ~Game() = default;
     virtual std::string getName() const = 0;
     Game();
     void setId (int i);
     int getId() const;
     virtual void printBoard() const = 0;
     virtual std::string input(std::string prompt) = 0;
};