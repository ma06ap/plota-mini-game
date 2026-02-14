//
// Created by matin on 2/14/26.
//

#ifndef PLOTA_MINI_GAME_OTHELLO_H
#define PLOTA_MINI_GAME_OTHELLO_H

#include "../Turn.h"
#include "../Game.h"
#include "../Location.h"

class Othello final : public Game {
    Turn turn;
    bool checkUp(Location& l,bool left,bool right) const;
    bool checkDown(Location& l,bool left,bool right) const;
    bool checkLeft(Location& l,bool up,bool down) const;
    bool checkRight(Location& l,bool up,bool down) const;
public:
    Othello();
    bool checkArround(Location& l) const;
    void addPiece(Location &l);
    void nextTurn();
    std::vector<Location> allowed();
    std::string getName() const;
    void printBoard() const;
    std::string getCurrentPlayer() const;
};



#endif //PLOTA_MINI_GAME_OTHELLO_H