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
    bool checkDirection(Location &l, int dr, int dc) const;
    bool checkArround(Location &l) const;
    void flipDirection(int r, int c, int dr, int dc);

public:
    Othello();
    std::string getName() const override;
    std::string input(std::string prompt) override;
    std::string getCurrentPlayer() const override;
    void printBoard() const override;

    void addPiece(Location &l);
    void nextTurn();
    std::string getWinner() const;
    std::vector<Location> allowed();
};

#endif // OTHELLO_H
