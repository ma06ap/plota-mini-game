#pragma once


#include "Board.h"
#include <string>


class Game {
protected:
     Board src;
     int id;

public:
     virtual std::string getName() const = 0;
     Game();
     void setId (int i);
     int getId() const;
};