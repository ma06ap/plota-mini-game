#include "board.h"

Board::Board(int rowNumber , int columnNumber) {
     board.resize(rowNumber);
     for (auto &temp : board) {
          temp.resize(columnNumber,nullptr);
     }
     // for (size_t i = 0; i < rowNumber; i++)
     // {
     //      std::cout << board.size();
     //      for (size_t j = 0; j < columnNumber; j++)
     //      {
     //           std::cout << board.at(i).size();
     //      }  
     // }
}


// ts

void Board::print() {
     for (auto t : board) {
          for (auto y : t) {
               std::cout << "*";
               // y.pr
          }
          std::cout << std::endl;
     }
}

