#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <string>
#include <vector>

class Game {
protected:
    int id;
    Board src;

public:
    Game();
    virtual ~Game() {}

    virtual std::string input(std::string prompt) = 0;
    virtual std::string getName() const = 0;

    virtual std::string getCurrentPlayer() const = 0;
    virtual void printBoard() const = 0;

    void setId(int i);
    int getId() const;
};

#endif // GAME_H
