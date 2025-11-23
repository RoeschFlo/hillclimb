#include "game.hpp"
#include <iostream>

Game::Game(GameState* initial) : state(initial) {}

Game::~Game() {
    delete state;
}

void Game::ChangeState(GameState* newState) {
    delete state;
    state = newState;
}

void Game::Update() {
    state->Update(*this);
}

void Game::Draw() {
    state->Draw();
}
