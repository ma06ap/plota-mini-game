#include "Connect-4.h"


ConnectFour::ConnectFour() : Game() , turn("Red","Yellow") {
    src = Board(6, 7);
    // history.push_back(Status(src, 0));
}

std::string ConnectFour::getName() const {
    return "Connect-4";
}

void ConnectFour::addColumn(int col) {
    col--;
    if (col < 0 || col >= src.getColumns()) {
        throw std::out_of_range("Column index out of range");
    }
    Piece *tmp = new Piece(turn.get());
    for (int i = src.getRows() - 1; i >= 0; --i) {
        if (src.getPiece(i,col)!=nullptr) {
            continue;
        }
        src.Add(tmp, i , col);
        break;
    }
}

void ConnectFour::nextTurn() {
    turn.switchTurn();
    // src.print();
}

bool ConnectFour::checkWin() {
    // remained
}

bool ConnectFour::checkDraw() {
    for (int i = 0; i < src.getColumns(); ++i) {
        if (src.getPiece(0,i) == nullptr) {
            return false;
        }
    }
    return true;
}

std::vector<int> ConnectFour::allowColumns() {
    std::vector<int> allowed;
    for (int i = 0; i < src.getColumns(); ++i) {
        if (src.getPiece(0,i) == nullptr) {
            allowed.push_back(i+1);
        }
    }
    return allowed;
}

bool ConnectFour::checkHorizontal() {
    for (int i = 0; i < src.getRows(); ++i) {
        int count = 0;
        for (int j = 0; j < src.getColumns(); ++j) {
            if (src.getPiece(i,j) != nullptr && src.getPiece(i,j)->getColor() == turn.get()) {
                count++;
                if (count == 4) {
                    location[0].setI(i);
                    location[0].setJ(j);
                    location[1].setI(i-1);
                    location[1].setJ(j);
                    location[2].setI(i-2);
                    location[2].setJ(j);
                    location[3].setI(i-3);
                    location[3].setJ(j);
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }
    return false;
}

bool ConnectFour::checkVertical() {
    for (int i = 0; i < src.getColumns(); ++i) {
        int count = 0;
        for (int j = 0; j < src.getRows(); ++j) {
            if (src.getPiece(j,i) != nullptr && src.getPiece(j,i)->getColor() == turn.get()) {
                count++;
                if (count == 4) {
                    location[0].setJ(j);
                    location[0].setI(i);
                    location[1].setJ(j-1);
                    location[1].setI(i);
                    location[2].setJ(j-2);
                    location[2].setI(i);
                    location[3].setJ(j-3);
                    location[3].setI(i);
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }
    return false;
}

bool ConnectFour::checkDiagonal1() {
    for (int i = 0; i < src.getRows() - 3; ++i) {
        for (int j = 0; j < src.getColumns() - 3; ++j) {
            if (src.getPiece(i,j) != nullptr && src.getPiece(i,j)->getColor() == turn.get() &&
                src.getPiece(i+1,j+1) != nullptr && src.getPiece(i+1,j+1)->getColor() == turn.get() &&
                src.getPiece(i+2,j+2) != nullptr && src.getPiece(i+2,j+2)->getColor() == turn.get() &&
                src.getPiece(i+3,j+3) != nullptr && src.getPiece(i+3,j+3)->getColor() == turn.get()) {
                location[0].setI(i);
                location[0].setJ(j);
                location[1].setI(i+1);
                location[1].setJ(j+1);
                location[2].setI(i+2);
                location[2].setJ(j+2);
                location[3].setI(i+3);
                location[3].setJ(j+3);
                return true;
            }
        }
    }
    return false;
}

bool ConnectFour::checkDiagonal2() {
    for (int i = 0; i < src.getRows() - 3; ++i) {
        for (int j = 3; j < src.getColumns(); ++j) {
            if (src.getPiece(i,j) != nullptr && src.getPiece(i,j)->getColor() == turn.get() &&
                src.getPiece(i+1,j-1) != nullptr && src.getPiece(i+1,j-1)->getColor() == turn.get() &&
                src.getPiece(i+2,j-2) != nullptr && src.getPiece(i+2,j-2)->getColor() == turn.get() &&
                src.getPiece(i+3,j-3) != nullptr && src.getPiece(i+3,j-3)->getColor() == turn.get()) {
                location[0].setI(i);
                location[0].setJ(j);
                location[1].setI(i+1);
                location[1].setJ(j-1);
                location[2].setI(i+2);
                location[2].setJ(j-2);
                location[3].setI(i+3);
                location[3].setJ(j-3);
                return true;
            }
        }
    }
    return false;
}
