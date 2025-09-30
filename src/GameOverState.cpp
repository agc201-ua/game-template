#include <GameOverState.hpp>

GameOverState::GameOverState(){}

void GameOverState::init(){
    background = LoadTexture("assets/background-night.png");
    base = LoadTexture("assets/base.png");
}

GameOverState::~GameOverState() {
    UnloadTexture(background);
    UnloadTexture(base);
}

void GameOverState::handleInput(){
    if(IsKeyPressed(KEY_SPACE)) {
        this->state_machine->add_state(std::make_unique<MainGameState>(), true);
    }
}

void GameOverState::update(float deltaTime){}


void GameOverState:: render() {
    BeginDrawing();
    //ClearBackground(RAYWHITE);
    DrawTexture(background, 0, 0, WHITE);
    DrawTexture(base, 0, GetScreenHeight() - base.height, WHITE);
    DrawText("Game Over!", 20, 180, 18, BLACK);
    DrawText("Press SPACE to restart", 20, 220, 18, BLACK);
    EndDrawing();
};