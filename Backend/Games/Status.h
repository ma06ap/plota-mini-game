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
    std::string turn;
    Status(std::string str, Board &board);
};


#endif //PLOTA_MINI_GAME_STATUS_H