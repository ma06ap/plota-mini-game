#ifndef CHECKERS_H
#define CHECKERS_H

#include "../Game.h"
#include "../Turn.h"
#include "../Piece.h"
#include "../Location.h"
#include <vector>
#include <string>
#include <cmath>

class Checkers : public Game {
private:
    Turn turn;
    bool pieceSelected;
    Location selectedPiece;
    bool mustContinueJump;

    bool canJump(const Location& from, const Location& to) const;
    bool canSimpleMove(const Location& from, const Location& to) const;
    std::vector<Location> getJumpMovesFrom(const Location& from) const;
    std::vector<Location> getSimpleMovesFrom(const Location& from) const;
    bool hasJumpMoves() const;
    void promoteToKingIfNeeded(const Location& loc);
    bool isPieceSelected() const { return pieceSelected; }

public:
    Checkers();
    std::string getName() const override;
    std::string input(std::string prompt) override;
    std::string getCurrentPlayer() const override;
    void printBoard() const override;

    std::string getBoard() const;
    std::string getWinner() const;
    std::vector<Location> allowedPieces();
    std::vector<Location> allowedMoves();
    void selectPiece(const Location& loc);
    void move(const Location& to);
    void nextTurn();
};

#endif // CHECKERS_H
