//
// Created by matin on 2/14/26.
//

#include "Othello.h"
#include <iostream>


bool Othello::checkUp(Location &l, bool left, bool right) const {
    // bool result = false;
    int i = l.getI();
    int j = l.getJ();
    i--;
    if (src.getPiece(i, j) != nullptr) {
        return true;
    }
    if (right && src.getPiece(i, j + 1) != nullptr) {
        return true;
    }
    if (left && src.getPiece(i, j - 1) != nullptr) {
        return true;
    }
    return false;
}

bool Othello::checkDown(Location &l, bool left, bool right) const {
    int i = l.getI();
    int j = l.getJ();
    i++;
    if (src.getPiece(i, j) != nullptr) {
        return true;
    }
    if (right && src.getPiece(i, j + 1) != nullptr) {
        return true;
    }
    if (left && src.getPiece(i, j - 1) != nullptr) {
        return true;
    }
    return false;
}

bool Othello::checkLeft(Location &l, bool up, bool down) const {
    int i = l.getI();
    int j = l.getJ();
    j--;
    if (src.getPiece(i, j) != nullptr) {
        return true;
    }
    if (up && src.getPiece(i - 1, j) != nullptr) {
        return true;
    }
    if (down && src.getPiece(i + 1, j) != nullptr) {
        return true;
    }
    return false;
}

bool Othello::checkRight(Location &l, bool up, bool down) const {
    int i = l.getI();
    int j = l.getJ();
    j++;
    if (src.getPiece(i, j) != nullptr) {
        return true;
    }
    if (up && src.getPiece(i - 1, j) != nullptr) {
        return true;
    }
    if (down && src.getPiece(i + 1, j) != nullptr) {
        return true;
    }
    return false;
}

Othello::Othello() : Game(), turn("Black","White") {
     src = Board(8, 8);
     // history.push_back(Status(src, 0));
    auto tmp1 = new Piece(turn.get());
    auto tmp2 = new Piece(turn.get());
    src.Add(tmp1, 3, 3);
    src.Add(tmp2, 4, 4);
    tmp1 = new Piece(turn.getOther());
    tmp2 = new Piece(turn.getOther());
    src.Add(tmp1, 3, 4);
    src.Add(tmp2, 4, 3);
}

bool Othello::checkArround(Location &l) const {
    int i = l.getI();
    int j = l.getJ();
    if (src.getPiece(i, j) != nullptr) {
        return false;
    }
    bool up=true;
    bool down=true;
    bool left=true;
    bool right=true;
    if (i == 0) {
        up = false;
    }
    if (i == 7) {
        down = false;
    }
    if (j == 0) {
        right = false;
    }
    if (j == 7) {
        left = false;
    }
    bool result[4](false);
    if (up) {
        result[0] = checkUp(l, left, right);
    }
    if (down) {
        result[1] = checkDown(l, left, right);
    }
    if (left) {
        result[2] = checkLeft(l, up, down);
    }
    if (right) {
        result[3] = checkRight(l, up, down);
    }
    return (result[0] || result[1] || result[2] || result[3]);
}

void Othello::addPiece(Location &l) {
    auto tmp = new Piece(turn.get());
    src.Add(tmp, l.getI(), l.getJ());
    // history.push_back(Status(src, 0));
    // turn.switchTurn();
    // change color of pieces that should be changed
     int i = l.getI();
     int j = l.getJ();
     std::string opponentColor = turn.getOther();

     // Check all 8 directions
     int directions[8][2] = {
         {-1, 0},  // up
         {1, 0},   // down
         {0, -1},  // left
         {0, 1},   // right
         {-1, -1}, // up-left
         {-1, 1},  // up-right
         {1, -1},  // down-left
         {1, 1}    // down-right
     };

     for (int dir = 0; dir < 8; ++dir) {
         int di = directions[dir][0];
         int dj = directions[dir][1];

         int x = i + di;
         int y = j + dj;

         std::vector<Location> piecesToFlip;

         // Move in the direction and check for opponent pieces
         while (x >= 0 && x < src.getRows() && y >= 0 && y < src.getColumns()) {
             Piece* piece = src.getPiece(x, y);

             if (piece == nullptr) {
                 break;
             }

             if (piece->getColor() == opponentColor) {
                 piecesToFlip.push_back(Location(x, y));
                 x += di;
                 y += dj;
             } else if (piece->getColor() == turn.get()) {
                 for (const auto& loc : piecesToFlip) {
                     src.getPiece(loc.getI(), loc.getJ())->setColor(turn.get());
                 }
                 break;
             } else {
                 break;
             }
         }
     }
}

void Othello::nextTurn() {
    turn.switchTurn();
}

std::string Othello::getWinner() const {
    int b = 0;
    int w = 0;
    for (int i = 0; i < src.getRows(); ++i) {
        for (int j = 0; j < src.getColumns(); ++j) {
            Piece* piece = src.getPiece(i, j);
            if (piece != nullptr) {
                if (piece->getColor() == "Black") {
                    b++;
                } else {
                    w++;
                }
            }
        }
    }
    if (b == w) {
        return "Draw";
    } else if (b > w) {
        return ("Black " + std::to_string(b) + " " + std::to_string(w));
    } else {
        return ("White " + std::to_string(w) + " " + std::to_string(b));
    }
}

std::vector<Location> Othello::allowed() {
    std::vector<Location> result;
    std::string currentColor = turn.get();
    std::string opponentColor = turn.getOther();

    // Check all empty cells
    for (int row = 0; row < src.getRows(); ++row) {
        for (int col = 0; col < src.getColumns(); ++col) {
            // Skip if cell is not empty
            if (src.getPiece(row, col) != nullptr) {
                continue;
            }

            bool isValid = false;

            // Check all 8 directions
            int directions[8][2] = {
                {-1, 0},  // up
                {1, 0},   // down
                {0, -1},  // left
                {0, 1},   // right
                {-1, -1}, // up-left
                {-1, 1},  // up-right
                {1, -1},  // down-left
                {1, 1}    // down-right
            };

            for (int dir = 0; dir < 8; ++dir) {
                int di = directions[dir][0];
                int dj = directions[dir][1];

                int i = row + di;
                int j = col + dj;

                bool foundOpponent = false;

                // Move in the direction and check for opponent pieces
                while (i >= 0 && i < src.getRows() && j >= 0 && j < src.getColumns()) {
                    Piece* piece = src.getPiece(i, j);

                    if (piece == nullptr) {
                        // Empty cell, can't flip
                        break;
                    }

                    if (piece->getColor() == opponentColor) {
                        // Found opponent piece
                        foundOpponent = true;
                        i += di;
                        j += dj;
                    } else if (piece->getColor() == currentColor) {
                        // Found our piece
                        if (foundOpponent) {
                            // Valid move: we can flip opponent pieces
                            isValid = true;
                        }
                        break;
                    } else {
                        break;
                    }
                }

                if (isValid) {
                    break;
                }
            }

            if (isValid) {
                result.push_back(Location(row, col));
            }
        }
    }

    return result;
}

std::string Othello::getName() const {
    return "Othello";
}

void Othello::printBoard() const {
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
            } else if (piece->getColor() == "Black") {
                std::cout << "B ";
            } else {
                std::cout << "W ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::string Othello::getCurrentPlayer() const {
    return turn.get();
}

std::string Othello::input(std::string prompt) {
    char ii = prompt[4];
    char jj = prompt[6];
    int i = ii - '0';
    int j = jj - '0';
    Location l(i, j);
    this->addPiece(l);
    this->nextTurn();
    auto tmp = this->allowed();
    if (tmp.empty()) {
        this->nextTurn();
        tmp = this->allowed();
        if (tmp.empty()) {
            return getWinner();
        }
    }
    std::string result;
    for (auto &loc : tmp) {
        result += std::to_string(loc.getI()) +" " + std::to_string(loc.getJ()) + " ";
    }
    return (turn.get() + " " + result);
}

