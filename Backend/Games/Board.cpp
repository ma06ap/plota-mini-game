#include "Board.h"

Board::Board(int rowNumber , int columnNumber) {
     board.resize(rowNumber);
     for (auto &temp : board) {
          temp.resize(columnNumber,nullptr);
     }
}


void Board::Add(Piece* piece , int rowNumber , int columnNumber) {
     board.at(rowNumber).at(columnNumber) = piece;
}

void Board::Delete (int rowNumber , int columnNumber) {
     auto temp = board.at(rowNumber).at(columnNumber);
     delete temp;
     board.at(rowNumber).at(columnNumber) = nullptr;
}

void Board::Move (int fromRow , int fromColumn , int toRow , int toColumn) {;
     auto temp = board.at(fromRow).at(fromColumn);
     board.at(toRow).at(toColumn) = temp;
     board.at(fromRow).at(fromColumn) = nullptr;
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

