#include <vector>

#include "piece.h"


class Board {
private:
     std::vector<std::vector<Piece*>> board;     
public:
     Board (int rowNumber , int columnNumber);

};