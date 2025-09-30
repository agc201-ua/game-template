#pragma once
#include <GameState.hpp>
#include <StateMachine.hpp>
#include <MainGameState.hpp>
#include <iostream>

class GameOverState : public GameState
{
    public:
        GameOverState();
        ~GameOverState();

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
        Texture2D background;
        Texture2D base;
};