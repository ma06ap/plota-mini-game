#pragma once
#include <vector>

//
#include <iostream>
//


#include "piece.h"


class Board {
private:
     std::vector<std::vector<Piece*>> board;     
public:
     Board (int rowNumber , int columnNumber);
     Add (Piece* piece , int rowNumber , int columnNumber);
     deletePiece (int rowNumber , int columnNumber);


     // these methods just for test
     void print();
};