#ifndef CONNECT_FOUR_H
#define CONNECT_FOUR_H

#include "../Game.h"
#include "../Turn.h"
#include "../Piece.h"
#include "../Location.h"
#include <vector>
#include <string>
#include <stdexcept>

class ConnectFour : public Game {
private:
    Turn turn;
    Location location[4];

public:
    ConnectFour();
    std::string getName() const override;
    std::string input(std::string prompt) override;
    std::string getCurrentPlayer() const override;
    void printBoard() const override;

    void addColumn(int col);
    void nextTurn();
    bool checkWin();
    bool checkDraw();
    std::vector<int> allowColumns();
    bool checkHorizontal();
    bool checkVertical();
    bool checkDiagonal1();
    bool checkDiagonal2();
};

#endif // CONNECT_FOUR_H
