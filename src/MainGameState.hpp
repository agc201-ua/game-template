#pragma once
#include <GameState.hpp>
#include <iostream>
#include <deque>

struct Bird {
    int x;
    int y;
    int vy;
};

struct PipePair { 
    Rectangle top, bot;
    bool scored=false;
};

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
        Bird player;
        std::deque<PipePair> pipes;
        const float PIPE_GAP = 100;
        const float PIPE_SPEED = 100;
        float spawnTimer;
        float spawnEvery;
        const float PIPE_W = 32;
        const float PIPE_H = 320;
};