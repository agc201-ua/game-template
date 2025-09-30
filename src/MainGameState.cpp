#include <MainGameState.hpp>

MainGameState::MainGameState()
{
    this->player.x = 200;
    this->player.y = 200;
    this->player.vy = 0;
    this->puntos = 0;

    this->spawnTimer = 0.0f;
    this->spawnEvery = 3.0f;
}

void MainGameState::init()
{
    InitAudioDevice();
    this->flapSound = LoadSound("assets/audio/audio_wing.wav");
    this->pointSound = LoadSound("assets/audio/audio_point.wav");
    this->birdSprite = LoadTexture("assets/bluebird-upflap.png");
    this->pipeSprite = LoadTexture("assets/pipe-green.png");
    this->background = LoadTexture("assets/background-day.png");
    this->base = LoadTexture("assets/base.png");
    this->player.height = birdSprite.height;
    this->player.width = birdSprite.width;
    this->PIPE_H = pipeSprite.height;
    this->PIPE_W = pipeSprite.width;
    this->PIPE_GAP = this->player.height * 4.5f;

    this->player.x = 200;
    this->player.y = 200;
    this->player.vy = 0;
    this->puntos = 0;

    this->pipes.clear();
    this->spawnTimer = 0.0f;
}

MainGameState::~MainGameState() {
    UnloadTexture(this->background);
    UnloadTexture(this->base);
    UnloadSound(this->flapSound);
    UnloadSound(this->pointSound);
    CloseAudioDevice();
}

void MainGameState::handleInput()
{
    if(IsKeyPressed(KEY_SPACE))
    {
        this->player.vy = -220;
        PlaySound(this->flapSound);
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
        static_cast<float>(this->player.x - this->player.width / 2), 
        static_cast<float>(this->player.y - this->player.height / 2), 
        static_cast<float>(this->player.width),
        static_cast<float>(this->player.height)
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

    // Update score
    for(auto& pipe : this->pipes) {
        if(!pipe.scored && (pipe.top.x + PIPE_W) < this->player.x) {
            pipe.scored = true;
            this->puntos += 1;        
            PlaySound(this->pointSound);
        }
    }
}

void MainGameState::render()
{
    BeginDrawing();
    //ClearBackground(RAYWHITE);
    DrawTexture(background, 0, 0, WHITE);
    DrawTexture(base, 0, GetScreenHeight() - base.height, WHITE);
    DrawText("Bienvenido a Flappy Bird DCA", 20, 200, 18, BLACK);
    DrawTextureEx(this->birdSprite, {static_cast<float>(this->player.x - this->player.width / 2),
        static_cast<float>(this->player.y - this->player.height / 2)}, 0.f, 1.0f, WHITE);
    for(const auto& p : this->pipes) {
        DrawTextureEx(this->pipeSprite, {p.top.x + PIPE_W, p.top.y + PIPE_H}, 180.f, 1.0f, WHITE);
        DrawTextureEx(this->pipeSprite, {p.bot.x , p.bot.y}, 0.f, 1.0f, WHITE);
    }
    DrawText(("Puntos: " + std::to_string(this->puntos)).c_str(), 10, 10, 20, BLACK);
    EndDrawing();
}