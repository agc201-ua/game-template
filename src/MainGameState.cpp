#include <MainGameState.hpp>

MainGameState::MainGameState()
{
    this->player.x = 200;
    this->player.y = 200;
    this->player.vy = 0;
    this->puntos = 0;
    this->pipeSpeed = PIPE_SPEED;

    this->spawnTimer = 0.0f;
    this->spawnEvery = 3.0f;
}

void MainGameState::init()
{
    InitAudioDevice();
    this->flapSound = LoadSound("assets/audio/audio_wing.wav");
    this->pointSound = LoadSound("assets/audio/audio_point.wav");
    this->hitSound = LoadSound("assets/audio/audio_hit.wav");
    this->dieSound = LoadSound("assets/audio/audio_die.wav");
    
    //this->birdSprite = LoadTexture("assets/bluebird-upflap.png");
    this->birdDownFlap = LoadTexture("assets/bluebird-downflap.png");
    this->birdMidFlap  = LoadTexture("assets/bluebird-midflap.png");
    this->birdUpFlap   = LoadTexture("assets/bluebird-upflap.png");
    currentBirdFrame = birdMidFlap;

    this->pipeSprite = LoadTexture("assets/pipe-green.png");
    this->background = LoadTexture("assets/background-day.png");
    this->base = LoadTexture("assets/base.png");
    for(int i = 0; i < 10; i++) {
        std::string path = "assets/" + std::to_string(i) + ".png";
        numberSprites[i] = LoadTexture(path.c_str());
    }

    //this->player.height = birdSprite.height;
    //this->player.width = birdSprite.width;
    this->player.width  = birdMidFlap.width;
    this->player.height = birdMidFlap.height;
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
    UnloadSound(this->hitSound);
    UnloadSound(this->dieSound);
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

    // Increase pipe speed based on score
    pipeSpeed += this->puntos * 0.01f;
    // Move pipes
    for(auto& pipe : this->pipes) {
        pipe.top.x -= pipeSpeed * deltaTime;
        pipe.bot.x -= pipeSpeed * deltaTime;
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

    if (this->isDead) {
        this->deathTimer -= deltaTime;
        if (this->deathTimer <= 0.0f) {
            this->state_machine->add_state(std::make_unique<GameOverState>(), true);
        }
        return;
    }

    // Check collisions
    for(const auto& pipe : this->pipes) {
        if(CheckCollisionRecs(birdRect, pipe.top) || CheckCollisionRecs(birdRect, pipe.bot)) {
            this->isDead = true;
            PlaySound(this->hitSound);
            PlaySound(this->dieSound);
            this->deathTimer = 0.7f; // delay before switching to Game Over state
            std::cout << "Collision detected!" << std::endl;
        }
    }

    // Check if bird is out of screen
    for(const auto& pipe : this->pipes) {
        if(player.y < 0 || player.y > GetScreenHeight()) {
            this->isDead = true;
            PlaySound(this->dieSound);
            this->deathTimer = 0.7f;
            std::cout << "The bird is dead!" << std::endl;
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

    //Check animations
    this->animationTimer += deltaTime;
    if(this->animationTimer >= this->animationSpeed) {
        this->animationTimer = 0.0f;
        this->frameIndex = (this->frameIndex + 1) % 3; // 3 frames: down, mid, up
        switch(this->frameIndex) {
            case 0:
                currentBirdFrame = birdDownFlap;
                break;
            case 1:
                currentBirdFrame = birdMidFlap;
                break;
            case 2:
                currentBirdFrame = birdUpFlap;
                break;
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
    /*DrawTextureEx(this->birdSprite, {static_cast<float>(this->player.x - this->player.width / 2),
        static_cast<float>(this->player.y - this->player.height / 2)}, 0.f, 1.0f, WHITE);*/
    DrawTextureEx(this->currentBirdFrame, {static_cast<float>(this->player.x - this->player.width / 2),
        static_cast<float>(this->player.y - this->player.height / 2)}, 0.f, 1.0f, WHITE);
    for(const auto& p : this->pipes) {
        DrawTextureEx(this->pipeSprite, {p.top.x + PIPE_W, p.top.y + PIPE_H}, 180.f, 1.0f, WHITE);
        DrawTextureEx(this->pipeSprite, {p.bot.x , p.bot.y}, 0.f, 1.0f, WHITE);
    }
    //DrawText(("Puntos: " + std::to_string(this->puntos)).c_str(), 10, 10, 20, BLACK);
    std::string score = std::to_string(this->puntos);
    int xOffset = 10;
    int yOffset = 10;
    for(char c : score) {
        int digit = c - '0'; // convertir char a int
        DrawTexture(numberSprites[digit], xOffset, yOffset, WHITE);
        xOffset += numberSprites[digit].width + 2; // separador de 2 px entre dígitos
    }
    EndDrawing();
}