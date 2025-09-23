#include <GameOverState.hpp>

GameOverState::GameOverState(){}

void GameOverState::init(){}

void GameOverState::handleInput(){
    if(IsKeyPressed(KEY_SPACE)) {
        this->state_machine->add_state(std::make_unique<MainGameState>(), true);
    }
}

void GameOverState::update(float deltaTime){}


void GameOverState:: render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Game Over!", 20, 180, 18, BLACK);
    DrawText("Press SPACE to restart", 20, 220, 18, BLACK);
    EndDrawing();
};