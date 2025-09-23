#include <StateMachine.hpp>
#include <MainGameState.hpp>
#include <memory>
#include <chrono>
#include "raylib.h"

int main()
{ 
    float delta_time = 0.0f;

    InitWindow(288, 512, "FlappyBird");

    SetTargetFPS(60);

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainGameState>(), false);
    state_machine.handle_state_changes(delta_time);

    while (!state_machine.is_game_ending())
    {
        state_machine.handle_state_changes(delta_time = GetFrameTime());
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();       
    }

    CloseWindow();

    return 0;
}