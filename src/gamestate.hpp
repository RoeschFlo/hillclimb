#pragma once
#include <raylib.h>


class Game;

class GameState{
public:
    virtual ~GameState() = default;

    virtual void Update(Game& game) = 0;
    virtual void Draw() = 0; //=0 muss in kind klasse überschrieben werden!

};

class MainMenueState:public GameState{
public:
    void Update(Game& game) override;
    void Draw() override;
};

class PlayingState : public GameState {
public:
    void Update(Game& game) override;
    void Draw() override;
};