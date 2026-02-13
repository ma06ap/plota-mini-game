#pragma once

#include "../Game.h"
#include "../Turn.h"


class ConnectFour : public Game {
    Turn turn;
public:
    ConnectFour();
    std::string getName() const override;
    void addColumn(int col);


};