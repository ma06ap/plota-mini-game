#ifndef PLOTA_MINI_GAME_LOCATION_H
#define PLOTA_MINI_GAME_LOCATION_H


class Location {
    int i;
    int j;
public:
    Location(int i, int j) : i(i), j(j) {}
    Location() : i(0) , j(0) {}
    int getI() const { return i; }
    int getJ() const { return j; }
    void setI(int i) { this->i = i; }
    void setJ(int j) { this->j = j; }
};


#endif //PLOTA_MINI_GAME_LOCATION_H
