#include "Game.h"

Game::Game() : src(8,8) {}


void Game::setId (int i) {
    id = i;
}


int Game::getId() const { return id; };