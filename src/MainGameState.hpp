#pragma once
#include <GameState.hpp>
#include <StateMachine.hpp>
#include <GameOverState.hpp>
#include <iostream>
#include <deque>

struct Bird {
    int x;
    int y;
    int vy;
    int height;
    int width;
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
        float PIPE_GAP;
        const float PIPE_SPEED = 100;
        float spawnTimer;
        float spawnEvery;
        float PIPE_W;
        float PIPE_H;
        int puntos;
        Texture2D birdSprite;
        Texture2D pipeSprite;
};