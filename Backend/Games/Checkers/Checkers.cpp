//
// Created by matin on 2/14/26.
//

#include "Checkers.h"
#include <iostream>

Checkers::Checkers() : Game(), turn("White","Black"), pieceSelected(false), mustContinueJump(false) {
    src = Board(8, 8);


    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < src.getColumns(); ++j) {
            if ((i + j) % 2 == 1) {
                auto tmp = new Piece(turn.get());
                src.Add(tmp, i, j);
            }
        }
    }
    for (int i = 5; i < src.getRows(); ++i) {
        for (int j = 0; j < src.getColumns(); ++j) {
            if ((i + j) % 2 == 1) {
                auto tmp = new Piece(turn.getOther());
                src.Add(tmp, i, j);
            }
        }
    }
}

std::string Checkers::getName() const {
    return "Checkers";
}

std::string Checkers::getCurrentPlayer() const {
    return turn.get();
}

std::string Checkers::input(std::string prompt) {


    if (prompt == "start") {
        auto pieces = allowedPieces();
        if (pieces.empty()) {
            return getWinner();
        }
        std::string result = turn.get() + " ";
        for (const auto& loc : pieces) {
            result += std::to_string(loc.getI()) + " " + std::to_string(loc.getJ()) + " ";
        }
        return result;
    }

    if (prompt.substr(0, 6) == "select") {
        char ii = prompt[7];
        char jj = prompt[9];
        int i = ii - '0';
        int j = jj - '0';
        Location loc(i, j);

        selectPiece(loc);

        auto moves = allowedMoves();
        if (moves.empty()) {
            return "No moves available";
        }

        std::string result = turn.get() + " selected ";
        for (const auto& move : moves) {
            result += std::to_string(move.getI()) + " " + std::to_string(move.getJ()) + " ";
        }
        return result;
    }

    if (prompt.substr(0, 4) == "move") {

        char ii = prompt[5];
        char jj = prompt[7];
        int i = ii - '0';
        int j = jj - '0';
        Location to(i, j);

        move(to);


        if (isPieceSelected()) {

            auto nextMoves = allowedMoves();
            if (nextMoves.empty()) {
                nextTurn();
            } else {
                std::string result = turn.get() + " continue ";
                for (const auto& move : nextMoves) {
                    result += std::to_string(move.getI()) + " " + std::to_string(move.getJ()) + " ";
                }
                return result;
            }
        }

        nextTurn();
        auto pieces = allowedPieces();
        if (pieces.empty()) {
            return getWinner();
        }

        std::string result = turn.get() + " ";
        for (const auto& loc : pieces) {
            result += std::to_string(loc.getI()) + " " + std::to_string(loc.getJ()) + " ";
        }
        return result;
    }

    if (prompt == "getboard") {
        return getBoard();
    }

    return "Invalid command";
}

std::string Checkers::getBoard() const {
    std::string result;
    for (int i = 0; i < src.getRows(); ++i) {
        for (int j = 0; j < src.getColumns(); ++j) {
            Piece* piece = src.getPiece(i, j);
            if (piece != nullptr) {
                result += std::to_string(i) + " " + std::to_string(j) + " ";
                result += piece->getColor() + " ";
                result += piece->getRole() + " ";
            }
        }
    }
    return result;
}

std::string Checkers::getWinner() const {
    std::string loser = turn.get();
    std::string winner = turn.getOther();
    return winner + " Win";
}



bool Checkers::canJump(const Location& from, const Location& to) const {
    int fromRow = from.getI();
    int fromCol = from.getJ();
    int toRow = to.getI();
    int toCol = to.getJ();

    if (toRow < 0 || toRow >= src.getRows() || toCol < 0 || toCol >= src.getColumns()) {
        return false;
    }

    if (src.getPiece(toRow, toCol) != nullptr) {
        return false;
    }

    Piece* piece = src.getPiece(fromRow, fromCol);
    if (piece == nullptr) {
        return false;
    }

    if (piece->getColor() != turn.get()) {
        return false;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;


    if (std::abs(rowDiff) != 2 || std::abs(colDiff) != 2) {
        return false;
    }


    if (piece->getRole() != "King") {
        if (piece->getColor() == "White") {
            if (rowDiff != 2) return false;
        } else {
            if (rowDiff != -2) return false;
        }
    }


    int midRow = fromRow + rowDiff / 2;
    int midCol = fromCol + colDiff / 2;

    Piece* middlePiece = src.getPiece(midRow, midCol);
    if (middlePiece == nullptr) {
        return false;
    }
    if (middlePiece->getColor() == piece->getColor()) {
        return false;
    }

    return true;
}

bool Checkers::canSimpleMove(const Location& from, const Location& to) const {
    int fromRow = from.getI();
    int fromCol = from.getJ();
    int toRow = to.getI();
    int toCol = to.getJ();

    if (toRow < 0 || toRow >= src.getRows() || toCol < 0 || toCol >= src.getColumns()) {
        return false;
    }


    if (src.getPiece(toRow, toCol) != nullptr) {
        return false;
    }

    Piece* piece = src.getPiece(fromRow, fromCol);
    if (piece == nullptr) {
        return false;
    }

    if (piece->getColor() != turn.get()) {
        return false;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    if (std::abs(rowDiff) != 1 || std::abs(colDiff) != 1) {
        return false;
    }

    if (piece->getRole() != "King") {
        if (piece->getColor() == "White") {
            if (rowDiff != 1) return false;
        } else {
            if (rowDiff != -1) return false;
        }
    }

    return true;
}

std::vector<Location> Checkers::getJumpMovesFrom(const Location& from) const {
    std::vector<Location> jumps;
    int row = from.getI();
    int col = from.getJ();

    int directions[4][2] = {{2, 2}, {2, -2}, {-2, 2}, {-2, -2}};

    for (int i = 0; i < 4; ++i) {
        Location to(row + directions[i][0], col + directions[i][1]);
        if (canJump(from, to)) {
            jumps.push_back(to);
        }
    }

    return jumps;
}


std::vector<Location> Checkers::getSimpleMovesFrom(const Location& from) const {
    std::vector<Location> moves;
    int row = from.getI();
    int col = from.getJ();

    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (int i = 0; i < 4; ++i) {
        Location to(row + directions[i][0], col + directions[i][1]);
        if (canSimpleMove(from, to)) {
            moves.push_back(to);
        }
    }

    return moves;
}

bool Checkers::hasJumpMoves() const {
    for (int row = 0; row < src.getRows(); ++row) {
        for (int col = 0; col < src.getColumns(); ++col) {
            Piece* piece = src.getPiece(row, col);
            if (piece != nullptr && piece->getColor() == turn.get()) {
                Location loc(row, col);
                if (!getJumpMovesFrom(loc).empty()) {
                    return true;
                }
            }
        }
    }
    return false;
}


void Checkers::promoteToKingIfNeeded(const Location& loc) {
    int row = loc.getI();
    int col = loc.getJ();

    Piece* piece = src.getPiece(row, col);
    if (piece == nullptr || piece->getRole() == "King") {
        return;
    }


    if ((piece->getColor() == "White" && row == 7) ||
        (piece->getColor() == "Black" && row == 0)) {
        auto king = new Piece(piece->getColor(), "King");
        src.Delete(row, col);
        src.Add(king, row, col);
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

    bool jumpAvailable = hasJumpMoves();

    std::vector<Location> kingPieces;
    std::vector<Location> regularPieces;

    for (int row = 0; row < src.getRows(); ++row) {
        for (int col = 0; col < src.getColumns(); ++col) {
            Piece* piece = src.getPiece(row, col);
            if (piece != nullptr && piece->getColor() == turn.get()) {
                Location loc(row, col);

                bool hasMove = false;
                if (jumpAvailable) {
                    hasMove = !getJumpMovesFrom(loc).empty();
                } else {
                    hasMove = !getSimpleMovesFrom(loc).empty();
                }

                if (hasMove) {
                    if (piece->getRole() == "King") {
                        kingPieces.push_back(loc);
                    } else {
                        regularPieces.push_back(loc);
                    }
                }
            }
        }
    }

    if (!kingPieces.empty()) {
        return kingPieces;
    }

    return regularPieces;
}


void Checkers::selectPiece(const Location& loc) {
    int row = loc.getI();
    int col = loc.getJ();

    Piece* piece = src.getPiece(row, col);
    if (piece == nullptr) {
        return;
    }

    if (piece->getColor() != turn.get()) {
        return;
    }


    auto allowed = allowedPieces();
    bool isAllowed = false;
    for (const auto& allowedLoc : allowed) {
        if (allowedLoc.getI() == row && allowedLoc.getJ() == col) {
            isAllowed = true;
            break;
        }
    }

    if (!isAllowed) {
        return;
    }

    selectedPiece = loc;
    pieceSelected = true;
    // if (piece->getRole() == "King") {}
}


std::vector<Location> Checkers::allowedMoves() {
    if (!pieceSelected) {
        return {};
    }


    if (mustContinueJump) {
        return getJumpMovesFrom(selectedPiece);
    }


    auto jumps = getJumpMovesFrom(selectedPiece);
    if (!jumps.empty()) {
        return jumps;
    }


    return getSimpleMovesFrom(selectedPiece);
}


void Checkers::move(const Location& to) {
    if (!pieceSelected) {
        return;
    }


    auto moves = allowedMoves();
    bool isValid = false;
    for (const auto& move : moves) {
        if (move.getI() == to.getI() && move.getJ() == to.getJ()) {
            isValid = true;
            break;
        }
    }

    if (!isValid) {

        return;
    }

    int fromRow = selectedPiece.getI();
    int fromCol = selectedPiece.getJ();
    int toRow = to.getI();
    int toCol = to.getJ();


    int rowDiff = std::abs(toRow - fromRow);
    bool isJump = (rowDiff == 2);

    if (isJump) {

        int midRow = fromRow + (toRow - fromRow) / 2;
        int midCol = fromCol + (toCol - fromCol) / 2;
        src.Delete(midRow, midCol);

    }


    src.Move(fromRow, fromCol, toRow, toCol);

    promoteToKingIfNeeded(to);


    if (isJump) {
        selectedPiece = to;
        auto nextJumps = getJumpMovesFrom(to);
        if (!nextJumps.empty()) {
            mustContinueJump = true;
            return;
        }
    }


    pieceSelected = false;
    mustContinueJump = false;
}


void Checkers::nextTurn() {
    if (mustContinueJump) {

        return;
    }

    pieceSelected = false;
    turn.switchTurn();
}

void Checkers::printBoard() const {
    std::cout << "\n  ";
    for (int j = 0; j < src.getColumns(); ++j) {
        std::cout << j << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < src.getRows(); ++i) {
        std::cout << i << " ";
        for (int j = 0; j < src.getColumns(); ++j) {
            Piece* piece = src.getPiece(i, j);
            if (piece == nullptr) {
                std::cout << ". ";
            } else if (piece->getColor() == "White") {
                if (piece->getRole() == "King") {
                    std::cout << "W ";
                } else {
                    std::cout << "w ";
                }
            } else {
                if (piece->getRole() == "King") {
                    std::cout << "B ";
                } else {
                    std::cout << "b ";
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
