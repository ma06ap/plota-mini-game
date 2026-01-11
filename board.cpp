#include "board.h"

Board::Board(int rowNumber , int columnNumber) {
     board.resize(rowNumber);
     for (auto t : board) {
          t.resize(rowNumber);
     }
}


// ts

void Board::print() {
     for (auto t : board) {
          for (auto y : board) {
               std::cout << "*";
               // y.pr
          }
          std::cout << std::endl;
     }
}

