#include "Turn.h"

Turn::Turn(std::string turn1,std::string turn2): firstTurn(turn1) , secondTurn(turn2) {
     turn = true;
}

void Turn::switchTurn() {
     turn^=1;
};

std::string Turn::get() {
     if(turn) {
          return firstTurn;
     }
     return secondTurn;
}