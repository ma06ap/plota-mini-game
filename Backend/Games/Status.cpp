#include "Status.h"

Status::Status(std::string str, Board &board) : his(board.getRows(),board.getColumns()) , turn(str) {
    his.copy(board);
}
