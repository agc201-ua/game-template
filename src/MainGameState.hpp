#pragma once
#include <GameState.hpp>
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
        const int PIPE_GAP = 100;
        const int PIPE_SPEED = 100;
        float spawnTimer;
        float spawnEvery;
        const int PIPE_W = 32;
        const int PIPE_H = 320;
};