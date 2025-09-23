#include <MainGameState.hpp>

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
        this->player.vy = -150;
    }
}

void MainGameState::update(float deltaTime)
{
    // Update player
    const float gravedad = 400.0f;
    this->player.vy += gravedad * deltaTime;
    this->player.y += this->player.vy * deltaTime;
    
    // Spawn pipes
    this->spawnTimer += deltaTime;
    if(this->spawnTimer >= this->spawnEvery) {
        this->spawnTimer = 0.0f;
        float pipe_y_offset_top = GetRandomValue(PIPE_H/2, GetScreenHeight()/2);
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

    // Move pipes
    for(auto& pipe : this->pipes) {
        pipe.top.x -= PIPE_SPEED * deltaTime;
        pipe.bot.x -= PIPE_SPEED * deltaTime;
    }

    // Remove off-screen pipes
    if (!this->pipes.empty() && this->pipes.front().top.x + PIPE_W < 0) {
        this->pipes.pop_front();
    }

    Rectangle birdRect = { 
        static_cast<float>(this->player.x - 17), 
        static_cast<float>(this->player.y - 17), 
        34, 34 
    };

    // Check bird boundaries
    /*
    DrawRectanglePro(
        birdRect,
        {0, 0},          // origen en la esquina superior izquierda
        0.0f,            // sin rotación
        Fade(BLUE, 0.5f) // azul semitransparente
    );
    */ 
   // Easiest way to check bird boundaries
   //DrawRectangleLinesEx(birdRect, 2, RED);

    // Check collisions
    for(const auto& pipe : this->pipes) {
        if(CheckCollisionRecs(birdRect, pipe.top) || CheckCollisionRecs(birdRect, pipe.bot)) {
            std::cout << "Collision detected!" << std::endl;
            this->state_machine->add_state(std::make_unique<GameOverState>(), true);
        }
    }

    // Check if bird is out of screen
    for(const auto& pipe : this->pipes) {
        if(player.y < 0 || player.y > GetScreenHeight()) {
            std::cout << "The bird is dead!" << std::endl;
            this->state_machine->add_state(std::make_unique<GameOverState>(), true);   
        }
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