#pragma once

#ifdef __MINGW32__ || defined(__SWITCH__)
#include "../../engine/core/State.h"
#include "../../engine/graphics/Sprite.h"
#include "../../engine/graphics/AnimatedSprite.h"
#include "../../engine/utils/Log.h"
#include "../../engine/utils/Paths.h"
#include "../../engine/graphics/Button.h"
#else
#include <core/State.h>
#include <graphics/Sprite.h>
#include <graphics/AnimatedSprite.h>
#include <utils/Log.h>
#include <utils/Paths.h>
#include <graphics/Button.h>
#endif

class PlayState : public State {
public:
    static PlayState* instance;
    
    PlayState();
    ~PlayState() override;
    
    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;
    
    void renderDebug();
    void openSubState(SubState* subState) override;

private:
    Sprite* backgroundSprite;
    Sprite* groundSprite;
    AnimatedSprite* birdSprite;
    std::vector<Sprite*> pipes;

    float birdVelocity;
    int score;
    bool gameOver;

    void updateBird(float deltaTime);
    void flapBird();
    void spawnPipe();
    void updatePipes(float deltaTime);
    void checkCollisions();
    void restartGame();
};
