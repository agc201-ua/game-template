#include <MainGameState.hpp>
#include <iostream>
extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState()
{
}

void MainGameState::init()
{

}

void MainGameState::handleInput()
{

}

void MainGameState::update(float deltaTime)
{

}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Bienvenido a Flappy Bird DCA", 20, 200, 18, BLACK);
    EndDrawing();
}