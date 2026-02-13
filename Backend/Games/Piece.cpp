#include "Piece.h"

Piece::Piece (std::string _color) : color(_color) , role("default") {}

std::string Piece::getRole() const {
    return role;
}

std::string Piece::getColor() const {
    return color;
}

void Piece::copy(Piece &p) {
    p.color = color;
    p.role = role;
}
