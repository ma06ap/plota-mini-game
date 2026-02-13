#pragma once


#include "Board.h"
#include "Turn.h"
#include <string>


class Game {
protected:
     Board src;
     int id;
     Turn turn;
public:
     virtual std::string getName() const = 0;
     Game();
     void setId (int i);
     int getId() const;
};