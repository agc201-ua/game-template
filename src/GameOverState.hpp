#pragma once
#include <GameState.hpp>
#include <StateMachine.hpp>
#include <MainGameState.hpp>
#include <iostream>

class GameOverState : public GameState
{
    public:
        GameOverState();
        ~GameOverState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
};