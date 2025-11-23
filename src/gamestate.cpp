#include "gamestate.hpp"
#include "game.hpp"

//-----------------------------------------------------

void MainMenueState::Update(Game& game) {
    if (IsKeyPressed(KEY_ENTER)) {
        game.ChangeState(new PlayingState());
    }
}

void MainMenueState::Draw() {
    ClearBackground(BLACK);
    DrawText("HAUPTMENÜ", 200, 150, 40, WHITE);
    DrawText("Drücke ENTER um zu starten", 180, 250, 20, GRAY);
}
//-----------------------------------------------------
void PlayingState::Update(Game& game) {
    // Zurück ins Menü
    if (IsKeyPressed(KEY_A)) {
        game.ChangeState(new MainMenueState());
    }
}

void PlayingState::Draw() {
    ClearBackground(DARKBLUE);
    DrawText("IM SPIEL!", 250, 150, 40, YELLOW);
    DrawText("Drücke A um zurückzugehen", 150, 250, 20, WHITE);
}
