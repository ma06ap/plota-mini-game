#pragma once
#include <vector>

//
#include <iostream>
//


#include <oneapi/tbb/detail/_template_helpers.h>

#include "Piece.h"


class Board {
private:
     std::vector<std::vector<Piece*>> board;
     int rows, columns;
public:
     Board (int rowNumber , int columnNumber);
     void Add (Piece* piece , int rowNumber , int columnNumber);
     void Delete (int rowNumber , int columnNumber);
     void Move (int fromRow , int fromColumn , int toRow , int toColumn);
     void copy(Board &board);
     int getRows () { return rows; }
     int getColumns () { return columns; }
     Piece* getPiece (int row , int column);
     // these methods just for test
     void print();
};