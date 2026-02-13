#include "Piece.h"

Piece::Piece (std::string _color) : color(_color) , role("default") {}

Piece::Piece() {
}

Piece::Piece(std::string _color, std::string _role) {
    role = _role;
    color = _color;
}

std::string Piece::getRole() const {
    return role;
}

std::string Piece::getColor() const {
    return color;
}

void Piece::copy(Piece &p) {
    color = p.getColor();
    role = p.getRole();
}
