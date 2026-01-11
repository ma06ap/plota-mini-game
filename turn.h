#include <string>

class Turn {
private:
     std::string firstTurn;
     std::string secondTurn;
     bool turn;
public:
     Turn(std::string turn1,std::string turn2);
     void switchTurn();
     std::string get();

};