#pragma once

#include <thread>
#include <mutex>
#include "../Game.h"
#include "../Turn.h"
#include "../Location.h"


class ConnectFour final : public Game  {
    Turn turn;
    Location location[4];
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