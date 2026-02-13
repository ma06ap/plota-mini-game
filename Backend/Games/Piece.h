#pragma once
#include <string>


class Piece {
private:
     std::string color;
     std::string role;
public:
     Piece (std::string _color);
     Piece ();
     Piece (std::string _color, std::string _role);
     std::string getRole() const;
     std::string getColor() const;
     void copy(Piece &p);
};