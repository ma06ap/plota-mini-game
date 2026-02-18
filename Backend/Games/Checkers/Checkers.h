#ifndef CHECKERS_H
#define CHECKERS_H

#include "../Game.h"
#include "../Turn.h"
#include "../Piece.h"
#include "../Location.h"
#include <vector>
#include <string>
#include <sstream>

class Checkers : public Game {
public:
    struct Move {
        int toR, toC;
        int capR, capC;
    };

private:
    Turn turn;

    bool pieceSelected;
    int selRow, selCol;

    bool inChainCapture;
    int chainRow, chainCol;

    bool isKing(Piece* p) const;
    bool sameTeam(Piece* a, Piece* b) const;

    std::vector<Move> getCaptures(int r, int c) const;
    std::vector<Move> getSimpleMoves(int r, int c) const;
    bool hasAnyCapture(const std::string& color) const;
    bool hasAnyMove(const std::string& color) const;
    std::vector<std::pair<int,int>> getSelectablePieces() const;

    void promoteIfNeeded(int r, int c);
    int  countPieces(const std::string& color) const;
    std::string buildSelectState() const;
    std::string buildMoveState(int r, int c) const;
    std::string handlePostMove(int toR, int toC, bool wasCapture, bool wasPromoted);

public:
    Checkers();
    std::string getName()           const override;
    std::string getCurrentPlayer()  const override;
    void        printBoard()        const override;
    std::string input(std::string prompt) override;
};

#endif // CHECKERS_H
