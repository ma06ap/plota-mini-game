//
// Created by matin on 2/14/26.
//

#ifndef PLOTA_MINI_GAME_CHECKERS_H
#define PLOTA_MINI_GAME_CHECKERS_H

#include "../Game.h"
#include "../Turn.h"
#include "../Location.h"
#include <utility>

class Checkers final : public Game {
    Turn turn;
    bool lastMoveWasJump;
    Location lastJumpLocation;

    bool isValidSimpleMove(const Location& from, const Location& to) const;
    bool isValidJumpMove(const Location& from, const Location& to) const;
    bool canJumpFrom(const Location& loc) const;
    void promoteToKing(const Location& loc);

public:
    Checkers();
    std::string getName() const override;
    std::vector<std::pair<Location, Location>> allowed() const;
    void movePiece(Location& last,Location &next);
    void nextTurn();
    void printBoard() const override;
    std::string getCurrentPlayer() const;
    std::string input(std::string prompt) override;
};


#endif //PLOTA_MINI_GAME_CHECKERS_H