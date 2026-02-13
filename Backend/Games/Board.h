#pragma once
#include <vector>

//
#include <iostream>
//


#include "Piece.h"


class Board {
private:
     std::vector<std::vector<Piece*>> board;     
public:
     Board (int rowNumber , int columnNumber);
     void Add (Piece* piece , int rowNumber , int columnNumber);
     void Delete (int rowNumber , int columnNumber);
     void Move (int fromRow , int fromColumn , int toRow , int toColumn);


     // these methods just for test
     void print();
};