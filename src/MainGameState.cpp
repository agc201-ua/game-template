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

    this->spawnTimer = 0.0f;
    this->spawnEvery = 3.0f;
}

void MainGameState::init()
{
    this->player.x = 200;
    this->player.y = 200;
    this->player.vy = 0;

    this->pipes.clear();
    this->spawnTimer = 0.0f;
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
    const float gravedad = 400.0f;
    this->player.vy += gravedad * deltaTime;
    this->player.y += this->player.vy * deltaTime;
    
    this->spawnTimer += deltaTime;
    if(this->spawnTimer >= this->spawnEvery) {
        this->spawnTimer = 0.0f;
        int pipe_y_offset_top = GetRandomValue(PIPE_H/2, GetScreenHeight()/2);
        float startX = GetScreenWidth();
        Rectangle top = { 
            startX, -pipe_y_offset_top, 
            PIPE_W, PIPE_H 
        };
        Rectangle bot = {
            startX, (PIPE_H - pipe_y_offset_top) + GetRandomValue(PIPE_H/2, GetScreenWidth()/2),
            PIPE_W, PIPE_H 
        };

        PipePair new_pair = {top, bot};
        this->pipes.push_back(new_pair);
    }

    for(auto& pipe : this->pipes) {
        pipe.top.x -= PIPE_SPEED * deltaTime;
        pipe.bot.x -= PIPE_SPEED * deltaTime;
    }

    if (!this->pipes.empty() && this->pipes.front().top.x + PIPE_W < 0) {
        this->pipes.pop_front();
    }

}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Bienvenido a Flappy Bird DCA", 20, 200, 18, BLACK);
    DrawCircle(this->player.x, this->player.y, 17, RED);
    for(const auto& pipe : this->pipes) {
        DrawRectangleRec(pipe.top, GREEN);
        DrawRectangleRec(pipe.bot, GREEN);
    }
    EndDrawing();
}