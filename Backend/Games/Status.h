//
// Created by matin on 2/13/26.
//

#ifndef PLOTA_MINI_GAME_STATUS_H
#define PLOTA_MINI_GAME_STATUS_H
#include "Board.h"
#include "Turn.h"

class Status {
public:
    Board his;
    Turn turn;
    Status(std::string str1, std::string str2);
};


#endif //PLOTA_MINI_GAME_STATUS_H