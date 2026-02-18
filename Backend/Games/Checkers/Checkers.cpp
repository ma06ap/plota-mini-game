#include "Checkers.h"
#include <iostream>

Checkers::Checkers() : Game(), turn("White","Black"), pieceSelected(false), mustContinueJump(false) {
    src = Board(8, 8);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < src.getColumns(); ++j) {
            if ((i + j) % 2 == 1) src.Add(new Piece(turn.get()), i, j);
        }
    }
    for (int i = 5; i < src.getRows(); ++i) {
        for (int j = 0; j < src.getColumns(); ++j) {
            if ((i + j) % 2 == 1) src.Add(new Piece(turn.getOther()), i, j);
        }
    }
}

std::string Checkers::getName() const { return "Checkers"; }
std::string Checkers::getCurrentPlayer() const { return turn.get(); }

std::string Checkers::input(std::string prompt) {
    if (prompt == "start") {
        auto pieces = allowedPieces();
        if (pieces.empty()) return getWinner();
        std::string result = turn.get() + " ";
        for (const auto& loc : pieces) result += std::to_string(loc.getI()) + " " + std::to_string(loc.getJ()) + " ";
        return result;
    }

    if (prompt.substr(0, 6) == "select") {
        char ii = prompt[7];
        char jj = prompt[9];
        int i = ii - '0';
        int j = jj - '0';
        Location loc(i, j);
        if (pieceSelected) {
            auto moves = allowedMoves();
            bool isMove = false;
            for (const auto& moveLoc : moves) {
                if (moveLoc.getI() == i && moveLoc.getJ() == j) {
                    isMove = true;
                    break;
                }
            }

            if (isMove) {
                move(loc);
                if (isPieceSelected()) {
                    auto nextMoves = allowedMoves();
                    if (nextMoves.empty()) {
                        nextTurn();
                    } else {
                        std::string result = turn.get() + " continue ";
                        for (const auto& m : nextMoves) result += std::to_string(m.getI()) + " " + std::to_string(m.getJ()) + " ";
                        return result;
                    }
                } else {
                    nextTurn();
                    auto pieces = allowedPieces();
                    if (pieces.empty()) return getWinner();

                    std::string result = turn.get() + " ";
                    for (const auto& l : pieces) result += std::to_string(l.getI()) + " " + std::to_string(l.getJ()) + " ";
                    return result;
                }
            }
        }

        selectPiece(loc);
        auto moves = allowedMoves();
        if (moves.empty()) return "No moves available";

        std::string result = turn.get() + " selected ";
        for (const auto& move : moves) result += std::to_string(move.getI()) + " " + std::to_string(move.getJ()) + " ";
        return result;
    }

    if (prompt == "getboard") return getBoard();
    return "Invalid command";
}

std::string Checkers::getBoard() const {
    std::string result;
    for (int i = 0; i < src.getRows(); ++i) {
        for (int j = 0; j < src.getColumns(); ++j) {
            Piece* piece = src.getPiece(i, j);
            if (piece != nullptr) {
                result += std::to_string(i) + " " + std::to_string(j) + " " + piece->getColor() + " " + piece->getRole() + " ";
            }
        }
    }
    return result;
}

std::string Checkers::getWinner() const {
    return turn.getOther() + " Win";
}

bool Checkers::canJump(const Location& from, const Location& to) const {
    int fromRow = from.getI(), fromCol = from.getJ();
    int toRow = to.getI(), toCol = to.getJ();
    if (toRow < 0 || toRow >= src.getRows() || toCol < 0 || toCol >= src.getColumns()) return false;
    if (src.getPiece(toRow, toCol) != nullptr) return false;
    Piece* piece = src.getPiece(fromRow, fromCol);
    if (piece == nullptr || piece->getColor() != turn.get()) return false;
    int rowDiff = toRow - fromRow, colDiff = toCol - fromCol;
    if (std::abs(rowDiff) != 2 || std::abs(colDiff) != 2) return false;
    if (piece->getRole() != "King") {
        if (piece->getColor() == "White" && rowDiff != 2) return false;
        if (piece->getColor() == "Black" && rowDiff != -2) return false;
    }
    int midRow = fromRow + rowDiff / 2, midCol = fromCol + colDiff / 2;
    Piece* midPiece = src.getPiece(midRow, midCol);
    return (midPiece != nullptr && midPiece->getColor() != piece->getColor());
}

bool Checkers::canSimpleMove(const Location& from, const Location& to) const {
    int fromRow = from.getI(), fromCol = from.getJ();
    int toRow = to.getI(), toCol = to.getJ();
    if (toRow < 0 || toRow >= src.getRows() || toCol < 0 || toCol >= src.getColumns()) return false;
    if (src.getPiece(toRow, toCol) != nullptr) return false;
    Piece* piece = src.getPiece(fromRow, fromCol);
    if (piece == nullptr || piece->getColor() != turn.get()) return false;
    int rowDiff = toRow - fromRow, colDiff = toCol - fromCol;
    if (std::abs(rowDiff) != 1 || std::abs(colDiff) != 1) return false;
    if (piece->getRole() != "King") {
        if (piece->getColor() == "White" && rowDiff != 1) return false;
        if (piece->getColor() == "Black" && rowDiff != -1) return false;
    }
    return true;
}

std::vector<Location> Checkers::getJumpMovesFrom(const Location& from) const {
    std::vector<Location> jumps;
    int directions[4][2] = {{2, 2}, {2, -2}, {-2, 2}, {-2, -2}};
    for (auto& d : directions) {
        Location to(from.getI() + d[0], from.getJ() + d[1]);
        if (canJump(from, to)) jumps.push_back(to);
    }
    return jumps;
}

std::vector<Location> Checkers::getSimpleMovesFrom(const Location& from) const {
    std::vector<Location> moves;
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (auto& d : directions) {
        Location to(from.getI() + d[0], from.getJ() + d[1]);
        if (canSimpleMove(from, to)) moves.push_back(to);
    }
    return moves;
}

bool Checkers::hasJumpMoves() const {
    for (int r = 0; r < src.getRows(); ++r) {
        for (int c = 0; c < src.getColumns(); ++c) {
            Piece* p = src.getPiece(r, c);
            if (p && p->getColor() == turn.get()) {
                if (!getJumpMovesFrom(Location(r, c)).empty()) return true;
            }
        }
    }
    return false;
}

void Checkers::promoteToKingIfNeeded(const Location& loc) {
    Piece* piece = src.getPiece(loc.getI(), loc.getJ());
    if (piece && piece->getRole() != "King") {
        if ((piece->getColor() == "White" && loc.getI() == 7) ||
            (piece->getColor() == "Black" && loc.getI() == 0)) {
            src.Delete(loc.getI(), loc.getJ());
            src.Add(new Piece(piece->getColor(), "King"), loc.getI(), loc.getJ());
        }
    }
}

std::vector<Location> Checkers::allowedPieces() {
    std::vector<Location> pieces;
    if (mustContinueJump && pieceSelected) {
        if (!getJumpMovesFrom(selectedPiece).empty()) {
            pieces.push_back(selectedPiece);
            return pieces;
        }
    }
    bool jumpAvail = hasJumpMoves();
    for (int r = 0; r < src.getRows(); ++r) {
        for (int c = 0; c < src.getColumns(); ++c) {
            Piece* p = src.getPiece(r, c);
            if (p && p->getColor() == turn.get()) {
                Location loc(r, c);
                bool has = jumpAvail ? !getJumpMovesFrom(loc).empty() : !getSimpleMovesFrom(loc).empty();
                if (has) pieces.push_back(loc);
            }
        }
    }
    return pieces;
}

void Checkers::selectPiece(const Location& loc) {
    Piece* p = src.getPiece(loc.getI(), loc.getJ());
    if (!p || p->getColor() != turn.get()) return;
    auto allowed = allowedPieces();
    for (const auto& a : allowed) {
        if (a.getI() == loc.getI() && a.getJ() == loc.getJ()) {
            selectedPiece = loc;
            pieceSelected = true;
            return;
        }
    }
}

std::vector<Location> Checkers::allowedMoves() {
    if (!pieceSelected) return {};
    if (mustContinueJump) return getJumpMovesFrom(selectedPiece);
    auto jumps = getJumpMovesFrom(selectedPiece);
    return jumps.empty() ? getSimpleMovesFrom(selectedPiece) : jumps;
}

void Checkers::move(const Location& to) {
    if (!pieceSelected) return;
    int fR = selectedPiece.getI(), fC = selectedPiece.getJ();
    int tR = to.getI(), tC = to.getJ();

    if (std::abs(tR - fR) == 2) {
        src.Delete(fR + (tR - fR) / 2, fC + (tC - fC) / 2);
    }
    src.Move(fR, fC, tR, tC);
    promoteToKingIfNeeded(to);

    if (std::abs(tR - fR) == 2) {
        selectedPiece = to;
        if (!getJumpMovesFrom(to).empty()) {
            mustContinueJump = true;
            return;
        }
    }
    pieceSelected = false;
    mustContinueJump = false;
}

void Checkers::nextTurn() {
    if (mustContinueJump) return;
    pieceSelected = false;
    turn.switchTurn();
}

void Checkers::printBoard() const {}
