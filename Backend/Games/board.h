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


     // these methods just for test
     void print();
};