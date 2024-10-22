#pragma once
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"
#include "../../engine/Input.h"
#include <vector>

class PlayState : public State {
public:
    PlayState();
    ~PlayState();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

    void keyPressed(unsigned char key, int x, int y);

    void renderDebug();

private:
    static PlayState* instance;

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
