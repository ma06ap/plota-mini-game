#include "board.h"

Board::Board(int rowNumber , int columnNumber) {
     board.resize(rowNumber);
     for (auto t : board) {
          t.resize(columnNumber,nullptr);
     }
}

