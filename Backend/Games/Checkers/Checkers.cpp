//
// Created by matin on 2/14/26.
//

#include "Checkers.h"
#include <iostream>

Checkers::Checkers() : Game(), turn("White","Black"), lastMoveWasJump(false) {
     src = Board(8, 8);
     // history.push_back(Status(src, 0));
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
    // in
}

bool Checkers::isValidSimpleMove(const Location& from, const Location& to) const {
    int fromRow = from.getI();
    int fromCol = from.getJ();
    int toRow = to.getI();
    int toCol = to.getJ();

    // Check bounds
    if (toRow < 0 || toRow >= src.getRows() || toCol < 0 || toCol >= src.getColumns()) {
        return false;
    }

    // Destination must be empty
    if (src.getPiece(toRow, toCol) != nullptr) {
        return false;
    }

    Piece* piece = src.getPiece(fromRow, fromCol);
    if (piece == nullptr) {
        return false;
    }

    // Check if it's the current player's piece
    if (piece->getColor() != turn.get()) {
        return false;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = std::abs(toCol - fromCol);

    // Must move diagonally (column difference must be 1)
    if (colDiff != 1) {
        return false;
    }

    // Regular pieces can only move forward
    if (piece->getRole() != "King") {
        if (piece->getColor() == "White") {
            // White moves down (increasing row)
            if (rowDiff != 1) {
                return false;
            }
        } else {
            // Black moves up (decreasing row)
            if (rowDiff != -1) {
                return false;
            }
        }
    } else {
        // Kings can move in both directions
        if (std::abs(rowDiff) != 1) {
            return false;
        }
    }

    return true;
}

bool Checkers::isValidJumpMove(const Location& from, const Location& to) const {
    int fromRow = from.getI();
    int fromCol = from.getJ();
    int toRow = to.getI();
    int toCol = to.getJ();

    // Check bounds
    if (toRow < 0 || toRow >= src.getRows() || toCol < 0 || toCol >= src.getColumns()) {
        return false;
    }

    // Destination must be empty
    if (src.getPiece(toRow, toCol) != nullptr) {
        return false;
    }

    Piece* piece = src.getPiece(fromRow, fromCol);
    if (piece == nullptr) {
        return false;
    }

    // Check if it's the current player's piece
    if (piece->getColor() != turn.get()) {
        return false;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = std::abs(toCol - fromCol);

    // Jump must be exactly 2 squares diagonally
    if (colDiff != 2 || std::abs(rowDiff) != 2) {
        return false;
    }

    // Regular pieces can only jump forward
    if (piece->getRole() != "King") {
        if (piece->getColor() == "White") {
            if (rowDiff != 2) {
                return false;
            }
        } else {
            if (rowDiff != -2) {
                return false;
            }
        }
    }

    // Check if there's an opponent piece to jump over
    int midRow = fromRow + rowDiff / 2;
    int midCol = fromCol + (toCol - fromCol) / 2;

    Piece* middlePiece = src.getPiece(midRow, midCol);
    if (middlePiece == nullptr) {
        return false;
    }

    // Must jump over opponent's piece
    if (middlePiece->getColor() == piece->getColor()) {
        return false;
    }

    return true;
}

bool Checkers::canJumpFrom(const Location& loc) const {
    int row = loc.getI();
    int col = loc.getJ();

    Piece* piece = src.getPiece(row, col);
    if (piece == nullptr || piece->getColor() != turn.get()) {
        return false;
    }

    // Check all 4 diagonal jump directions
    int jumpDirections[4][2] = {
        {2, 2},   // down-right
        {2, -2},  // down-left
        {-2, 2},  // up-right
        {-2, -2}  // up-left
    };

    for (int dir = 0; dir < 4; ++dir) {
        Location to(row + jumpDirections[dir][0], col + jumpDirections[dir][1]);
        if (isValidJumpMove(loc, to)) {
            return true;
        }
    }

    return false;
}

void Checkers::promoteToKing(const Location& loc) {
    int row = loc.getI();
    int col = loc.getJ();

    Piece* piece = src.getPiece(row, col);
    if (piece == nullptr) {
        return;
    }

    // White pieces are promoted at row 7, Black at row 0
    if ((piece->getColor() == "White" && row == 7) ||
        (piece->getColor() == "Black" && row == 0)) {
        // Create a new king piece
        auto king = new Piece(piece->getColor(), "King");
        src.Delete(row, col);
        src.Add(king, row, col);
    }
}

std::vector<std::pair<Location, Location>> Checkers::allowed() const {
    std::vector<std::pair<Location, Location>> result;
    std::vector<std::pair<Location, Location>> jumpMoves;
    std::vector<std::pair<Location, Location>> simpleMoves;

    // If last move was a jump, only check for additional jumps from that location
    if (lastMoveWasJump && canJumpFrom(lastJumpLocation)) {
        int row = lastJumpLocation.getI();
        int col = lastJumpLocation.getJ();

        int jumpDirections[4][2] = {
            {2, 2}, {2, -2}, {-2, 2}, {-2, -2}
        };

        for (int dir = 0; dir < 4; ++dir) {
            Location to(row + jumpDirections[dir][0], col + jumpDirections[dir][1]);
            if (isValidJumpMove(lastJumpLocation, to)) {
                jumpMoves.push_back({lastJumpLocation, to});
            }
        }

        return jumpMoves;
    }

    // Check all pieces of current player
    for (int row = 0; row < src.getRows(); ++row) {
        for (int col = 0; col < src.getColumns(); ++col) {
            Piece* piece = src.getPiece(row, col);
            if (piece == nullptr || piece->getColor() != turn.get()) {
                continue;
            }

            Location from(row, col);

            // Check for jump moves (priority)
            int jumpDirections[4][2] = {
                {2, 2}, {2, -2}, {-2, 2}, {-2, -2}
            };

            for (int dir = 0; dir < 4; ++dir) {
                Location to(row + jumpDirections[dir][0], col + jumpDirections[dir][1]);
                if (isValidJumpMove(from, to)) {
                    jumpMoves.push_back({from, to});
                }
            }

            // Check for simple moves
            int simpleDirections[4][2] = {
                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
            };

            for (int dir = 0; dir < 4; ++dir) {
                Location to(row + simpleDirections[dir][0], col + simpleDirections[dir][1]);
                if (isValidSimpleMove(from, to)) {
                    simpleMoves.push_back({from, to});
                }
            }
        }
    }

    // Jumps are mandatory in checkers
    if (!jumpMoves.empty()) {
        return jumpMoves;
    }

    return simpleMoves;
}

void Checkers::movePiece(Location& last, Location& next) {
    int fromRow = last.getI();
    int fromCol = last.getJ();
    int toRow = next.getI();
    int toCol = next.getJ();

    // Validate move is in allowed moves
    auto allowedMoves = allowed();
    bool found = false;
    for (const auto& move : allowedMoves) {
        if (move.first.getI() == fromRow && move.first.getJ() == fromCol &&
            move.second.getI() == toRow && move.second.getJ() == toCol) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Invalid move!" << std::endl;
        return;
    }

    // Check if this is a jump move
    int rowDiff = std::abs(toRow - fromRow);
    bool isJump = (rowDiff == 2);

    if (isJump) {
        // Remove the jumped piece
        int midRow = fromRow + (toRow - fromRow) / 2;
        int midCol = fromCol + (toCol - fromCol) / 2;
        src.Delete(midRow, midCol);

        // Move the piece
        src.Move(fromRow, fromCol, toRow, toCol);

        // Promote to king if reached the end
        promoteToKing(next);

        // Check if more jumps are possible
        lastJumpLocation = next;
        if (canJumpFrom(next)) {
            lastMoveWasJump = true;
            std::cout << "Multiple jump available! Continue jumping." << std::endl;
        } else {
            lastMoveWasJump = false;
        }
    } else {
        // Simple move
        src.Move(fromRow, fromCol, toRow, toCol);
        promoteToKing(next);
        lastMoveWasJump = false;
    }
}

void Checkers::nextTurn() {
    // Only switch turn if no more jumps are available
    if (!lastMoveWasJump) {
        turn.switchTurn();
    }
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
