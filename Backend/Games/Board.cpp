#include "Board.h"

Board::Board(int rowNumber , int columnNumber) {
     board.resize(rowNumber);
     for (auto &temp : board) {
          temp.resize(columnNumber,nullptr);
     }
     rows = rowNumber;
     columns = columnNumber;
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

void Board::copy(Board &board) {
     if (rows == board.getRows() && columns == board.getColumns()) {
          for (int i = 0; i < rows; i++) {
               for (int j = 0; j < columns; j++) {
                    auto temp2 = board.board.at(i).at(j);
                    if (temp2 != nullptr) {
                         Piece* newPiece = new Piece();
                         newPiece->copy(*temp2);
                         this->board.at(i).at(j) = newPiece;
                    } else {
                         this->board.at(i).at(j) = nullptr;
                    }
               }
          }
     }
     else {
          throw std::invalid_argument("Board does not have the same size as the board");
     }
}



// ts

Piece* Board::getPiece(int row, int column) {
     return board.at(row).at(column);
}

void Board::print() {
     for (auto t : board) {
          for (auto y : t) {
               if (y == nullptr) {
                    std::cout << "O";
               } else {
                    if (y->getColor() == "Red") {
                         std::cout << "R";
                    }
                    else {
                         std::cout << "Y";
                    }
               }
          }
          std::cout << std::endl;
     }
     std::cout << "-------------" <<std::endl;
}

