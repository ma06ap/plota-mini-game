//
// Created by matin on 2/14/26.
//

#ifndef PLOTA_MINI_GAME_CHECKERS_H
#define PLOTA_MINI_GAME_CHECKERS_H

#include "../Game.h"
#include "../Turn.h"
#include "../Location.h"
#include <vector>

class Checkers final : public Game {
    Turn turn;
    Location selectedPiece;
    bool pieceSelected;
    bool mustContinueJump;


    bool canJump(const Location& from, const Location& to) const;
    bool canSimpleMove(const Location& from, const Location& to) const;
    std::vector<Location> getJumpMovesFrom(const Location& from) const;
    std::vector<Location> getSimpleMovesFrom(const Location& from) const;
    bool hasJumpMoves() const;
    void promoteToKingIfNeeded(const Location& loc);

public:
    Checkers();
    std::string getName() const override;
    void printBoard() const override;
    std::string input(std::string prompt) override;

    std::vector<Location> allowedPieces();
    void selectPiece(const Location& loc);
    std::vector<Location> allowedMoves();
    void move(const Location& to);
    void nextTurn();

    std::string getCurrentPlayer() const;
    Location getSelectedPiece() const { return selectedPiece; }
    bool isPieceSelected() const { return pieceSelected; }

    std::string getBoard() const;
    std::string getWinner() const;
};


#endif //PLOTA_MINI_GAME_CHECKERS_H