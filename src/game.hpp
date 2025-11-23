#pragma once
#include "gamestate.hpp"

class Game {
private:
    GameState * state;
    

public:

    Game(GameState* initial);
    ~Game();

    void ChangeState(GameState* newState);

    void Update();
    void Draw();

};
