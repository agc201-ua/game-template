#include <MainGameState.hpp>
#include <iostream>
extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState()
{
    this->player.x = 200;
    this->player.y = 200;
    this->player.vy = 0;
}

void MainGameState::init()
{
    this->player.x = 200;
    this->player.y = 200;
    this->player.vy = 0;
}

void MainGameState::handleInput()
{
    if(IsKeyPressed(KEY_SPACE))
    {
        this->player.vy = -300;
    }
}

void MainGameState::update(float deltaTime)
{
    const int gravedad = 600;
    this->player.vy += gravedad * deltaTime;
    this->player.y += this->player.vy * deltaTime;
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Bienvenido a Flappy Bird DCA", 20, 200, 18, BLACK);
    DrawCircle(this->player.x, this->player.y, 17, RED);
    EndDrawing();
}