#include "Checkers.h"
#include <iostream>
#include <sstream>

Checkers::Checkers()
    : Game(),
    turn("Red", "Black"),
    pieceSelected(false), selRow(-1), selCol(-1),
    inChainCapture(false), chainRow(-1), chainCol(-1)
{
    src = Board(8, 8);

    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 8; ++c)
            if ((r + c) % 2 == 1)
                src.Add(new Piece("Black", "default"), r, c);

    for (int r = 5; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if ((r + c) % 2 == 1)
                src.Add(new Piece("Red", "default"), r, c);
}
std::string Checkers::getName()          const { return "Checkers"; }
std::string Checkers::getCurrentPlayer() const { return turn.get(); }
void        Checkers::printBoard()       const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = src.getPiece(r, c);
            if (!p)                          std::cout << ". ";
            else if (p->getColor() == "Red") std::cout << (isKing(p) ? "RK" : "R ");
            else                             std::cout << (isKing(p) ? "BK" : "B ");
        }
        std::cout << "\n";
    }
}

bool Checkers::isKing(Piece* p) const {
    return p && p->getRole() == "king";
}

bool Checkers::sameTeam(Piece* a, Piece* b) const {
    if (!a || !b) return false;
    return a->getColor() == b->getColor();
}

std::vector<Checkers::Move> Checkers::getCaptures(int r, int c) const {
    std::vector<Move> caps;
    Piece* p = src.getPiece(r, c);
    if (!p) return caps;
    std::vector<std::pair<int,int>> dirs;
    if (p->getColor() == "Red"   || isKing(p)) { dirs.push_back({-1,-1}); dirs.push_back({-1,+1}); }
    if (p->getColor() == "Black" || isKing(p)) { dirs.push_back({+1,-1}); dirs.push_back({+1,+1}); }

    for (auto [dr, dc] : dirs) {
        int mr = r + dr,     mc = c + dc;
        int lr = r + 2*dr,   lc = c + 2*dc;
        if (mr < 0 || mr > 7 || mc < 0 || mc > 7) continue;
        if (lr < 0 || lr > 7 || lc < 0 || lc > 7) continue;

        Piece* mid  = src.getPiece(mr, mc);
        Piece* land = src.getPiece(lr, lc);

        if (!mid)               continue;
        if (sameTeam(p, mid))  continue;
        if (land)               continue;

        caps.push_back({lr, lc, mr, mc});
    }
    return caps;
}

std::vector<Checkers::Move> Checkers::getSimpleMoves(int r, int c) const {
    std::vector<Move> moves;
    Piece* p = src.getPiece(r, c);
    if (!p) return moves;

    std::vector<std::pair<int,int>> dirs;
    if (p->getColor() == "Red"   || isKing(p)) { dirs.push_back({-1,-1}); dirs.push_back({-1,+1}); }
    if (p->getColor() == "Black" || isKing(p)) { dirs.push_back({+1,-1}); dirs.push_back({+1,+1}); }

    for (auto [dr, dc] : dirs) {
        int nr = r + dr, nc = c + dc;
        if (nr < 0 || nr > 7 || nc < 0 || nc > 7) continue;
        if (src.getPiece(nr, nc))                  continue;
        moves.push_back({nr, nc, -1, -1});
    }
    return moves;
}

bool Checkers::hasAnyCapture(const std::string& color) const {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Piece* p = src.getPiece(r, c);
            if (p && p->getColor() == color && !getCaptures(r, c).empty())
                return true;
        }
    return false;
}

bool Checkers::hasAnyMove(const std::string& color) const {
    if (hasAnyCapture(color)) return true;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Piece* p = src.getPiece(r, c);
            if (p && p->getColor() == color && !getSimpleMoves(r, c).empty())
                return true;
        }
    return false;
}

std::vector<std::pair<int,int>> Checkers::getSelectablePieces() const {
    std::string color    = turn.get();
    bool        mustCap  = hasAnyCapture(color);
    std::vector<std::pair<int,int>> result;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = src.getPiece(r, c);
            if (!p || p->getColor() != color) continue;

            if (mustCap) {
                if (!getCaptures(r, c).empty()) result.push_back({r, c});
            } else {
                if (!getSimpleMoves(r, c).empty()) result.push_back({r, c});
            }
        }
    }
    // add for force to king capture if it can
    if (mustCap) {
        int i,j;
        for (auto tmp : result) {
            i = tmp.first;
            j = tmp.second;
            if (src.getPiece(i,j)->getRole() == "King") {
                for (int t=0 ; t < result.size() ; t++) {
                    std::pair<int,int> loc(result.at(t).first,result.at(t).second);
                    if (src.getPiece(result.at(t).first,result.at(t).second)->getRole() != "King") {
                        result.erase(result.begin()+t);
                    } 
                }
            }
        }
    }
    return result;
}

void Checkers::promoteIfNeeded(int r, int c) {
    Piece* p = src.getPiece(r, c);
    if (!p) return;
    if (p->getColor() == "Red"   && r == 0) p->setRole("king");
    if (p->getColor() == "Black" && r == 7) p->setRole("king");
}

int Checkers::countPieces(const std::string& color) const {
    int n = 0;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Piece* p = src.getPiece(r, c);
            if (p && p->getColor() == color) ++n;
        }
    return n;
}

std::string Checkers::buildSelectState() const {
    auto pieces = getSelectablePieces();
    std::string res = turn.get();
    for (auto [r, c] : pieces)
        res += " " + std::to_string(r) + " " + std::to_string(c);
    return res;
}

std::string Checkers::buildMoveState(int r, int c) const {
    bool mustCap = hasAnyCapture(turn.get());
    auto moves   = mustCap ? getCaptures(r, c) : getSimpleMoves(r, c);
    std::string res = turn.get() + " selected "
                      + std::to_string(r) + " " + std::to_string(c);
    for (auto& m : moves)
        res += " " + std::to_string(m.toR) + " " + std::to_string(m.toC);
    return res;
}
std::string Checkers::handlePostMove(int toR, int toC,
                                     bool wasCapture, bool wasPromoted)
{
    if (wasCapture && !wasPromoted) {
        auto moreCaps = getCaptures(toR, toC);
        if (!moreCaps.empty()) {
            inChainCapture = true;
            chainRow = toR;  chainCol = toC;
            pieceSelected = true;
            selRow = toR;    selCol = toC;
            std::string res = turn.get() + " continue "
                              + std::to_string(toR) + " " + std::to_string(toC);
            for (auto& m : moreCaps)
                res += " " + std::to_string(m.toR) + " " + std::to_string(m.toC);
            return res;
        }
    }
    inChainCapture = false;
    pieceSelected  = false;
    selRow = selCol = chainRow = chainCol = -1;
    std::string opponent = turn.getOther();
    if (countPieces(opponent) == 0 || !hasAnyMove(opponent))
        return turn.get() + " Win";
    turn.switchTurn();
    return buildSelectState();
}

std::string Checkers::input(std::string prompt) {
    if (prompt == "start") {
        pieceSelected  = false;
        inChainCapture = false;
        selRow = selCol = chainRow = chainCol = -1;
        return buildSelectState();
    }

    if (prompt == "getboard") {
        std::string res;
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c) {
                Piece* p = src.getPiece(r, c);
                if (p)
                    res += std::to_string(r) + " " + std::to_string(c)
                           + " " + p->getColor()
                           + " " + p->getRole() + " ";
            }
        return res;
    }

    if (prompt.size() < 9 || prompt.substr(0, 7) != "select ")
        return "Invalid Command";

    int clickR, clickC;
    {
        std::istringstream iss(prompt.substr(7));
        if (!(iss >> clickR >> clickC)) return "Invalid Command";
        if (clickR < 0 || clickR > 7 || clickC < 0 || clickC > 7)
            return "Invalid Command";
    }

    std::string color  = turn.get();
    Piece*      clicked = src.getPiece(clickR, clickC);

    if (inChainCapture) {
        if (clickR == chainRow && clickC == chainCol) {
            return buildMoveState(chainRow, chainCol);
        }

        auto caps = getCaptures(chainRow, chainCol);
        for (auto& m : caps) {
            if (m.toR == clickR && m.toC == clickC) {
                src.Delete(m.capR, m.capC);
                src.Move(chainRow, chainCol, clickR, clickC);
                bool promoted = false;
                promoteIfNeeded(clickR, clickC);
                Piece* moved = src.getPiece(clickR, clickC);
                if (moved && moved->getRole() == "king") {
                    if ((color == "Red"   && clickR == 0) ||
                        (color == "Black" && clickR == 7))
                        promoted = true;
                }
                return handlePostMove(clickR, clickC, true, promoted);
            }
        }
        return "Invalid Move";
    }
    if (!pieceSelected) {
        if (!clicked || clicked->getColor() != color)
            return "Invalid Move";
        auto selectable = getSelectablePieces();
        bool ok = false;
        for (auto [sr, sc] : selectable)
            if (sr == clickR && sc == clickC) { ok = true; break; }
        if (!ok) return "Invalid Move";

        pieceSelected = true;
        selRow = clickR; selCol = clickC;
        return buildMoveState(clickR, clickC);
    }

    if (clickR == selRow && clickC == selCol) {
        pieceSelected = false;
        selRow = selCol = -1;
        return buildSelectState();
    }

    if (clicked && clicked->getColor() == color) {
        auto selectable = getSelectablePieces();
        bool ok = false;
        for (auto [sr, sc] : selectable)
            if (sr == clickR && sc == clickC) { ok = true; break; }

        if (ok) {
            selRow = clickR; selCol = clickC;
            return buildMoveState(clickR, clickC);
        }
        return buildMoveState(selRow, selCol);
    }

    {
        bool mustCap = hasAnyCapture(color);
        auto moves   = mustCap ? getCaptures(selRow, selCol)
                             : getSimpleMoves(selRow, selCol);

        for (auto& m : moves) {
            if (m.toR == clickR && m.toC == clickC) {
                bool wasCapture = (m.capR != -1);
                if (wasCapture) src.Delete(m.capR, m.capC);

                src.Move(selRow, selCol, clickR, clickC);

                bool promoted = false;
                {
                    Piece* p = src.getPiece(clickR, clickC);
                    bool wasKing = isKing(p);
                    promoteIfNeeded(clickR, clickC);
                    if (!wasKing && isKing(src.getPiece(clickR, clickC)))
                        promoted = true;
                }

                pieceSelected = false;
                return handlePostMove(clickR, clickC, wasCapture, promoted);
            }
        }
    }
    return "Invalid Move";
}
