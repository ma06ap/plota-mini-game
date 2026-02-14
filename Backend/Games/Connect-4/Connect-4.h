#pragma once

#include <thread>
#include <mutex>
#include "../Game.h"
#include "../Turn.h"


class ConnectFour : public Game {
    Turn turn;
public:
    ConnectFour();
    std::string getName() const override;
    void addColumn(int col);
    void nextTurn ();
    bool checkWin();
    bool checkDraw();
    std::vector<int> allowColumns ();
    bool checkHorizontal();
    bool checkVertical();
    bool checkDiagonal1();
    bool checkDiagonal2();
};