#include "Connect-4.h"


ConnectFour::ConnectFour() : Game() , turn("Red","Yellow") {
    src = Board(6, 7);
    // history.push_back(Status(src, 0));
}

std::string ConnectFour::getName() const {
    return "Connect-4";
}

void ConnectFour::addColumn(int col) {
    if (col < 0 || col >= src.getColumns()) {
        throw std::out_of_range("Column index out of range");
    }
    Piece *tmp = new Piece(turn.get());
    for (int i = src.getRows() - 1; i >= 0; --i) {
        if (src.getPiece(i,col)==nullptr) {
            continue;
        }
        src.Add(tmp, i + 1, col);
    }
}