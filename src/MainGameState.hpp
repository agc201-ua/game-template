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
        ~MainGameState();

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
        Texture2D pipeSprite;
        Texture2D background;
        Texture2D base;
        Sound flapSound;
        Sound pointSound;
        Sound hitSound;
        Sound dieSound;
        bool isDead = false;
        float deathTimer = 0.0f;
        Texture2D numberSprites[10];
        float pipeSpeed;
        Texture2D birdDownFlap;
        Texture2D birdMidFlap;
        Texture2D birdUpFlap;
        Texture2D currentBirdFrame;
        float animationTimer = 0.0f;
        float animationSpeed = 0.2f;
        int frameIndex = 0;
    };