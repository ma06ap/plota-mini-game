#ifndef OTHELLO_H
#define OTHELLO_H

#include "../Game.h"
#include "../Turn.h"
#include "../Piece.h"
#include "../Location.h"
#include <vector>
#include <string>

class Othello : public Game {
private:
    Turn turn;
    bool checkUp(Location &l, bool left, bool right) const;
    bool checkDown(Location &l, bool left, bool right) const;
    bool checkLeft(Location &l, bool up, bool down) const;
    bool checkRight(Location &l, bool up, bool down) const;

public:
    Othello();
    std::string getName() const override;
    std::string input(std::string prompt) override;
    std::string getCurrentPlayer() const override;
    void printBoard() const override;

    bool checkArround(Location &l) const;
    void addPiece(Location &l);
    void nextTurn();
    std::string getWinner() const;
    std::vector<Location> allowed();
};

#endif // OTHELLO_H
