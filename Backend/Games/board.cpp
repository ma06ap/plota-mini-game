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


Board::Add(Piece* piece , int rowNumber , int columnNumber) {
     board.at(rowNumber).at(columnNumber) = piece;
}

Board::deletePiece (int rowNumber , int columnNumber) {
     auto temp = board.at(rowNumber).at(columnNumber);
     delete temp;
     board.at(rowNumber).at(columnNumber) = nullptr;
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

