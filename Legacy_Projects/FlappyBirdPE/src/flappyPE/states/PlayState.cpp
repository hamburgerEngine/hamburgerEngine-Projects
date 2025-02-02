#include "../../../include/flappyPE/states/PlayState.h"
#include "../../../include/flappyPE/substates/PauseSubState.h"
#include "../../../include/engine/Engine.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>

PlayState* PlayState::instance = nullptr;

PlayState::PlayState() : backgroundSprite(nullptr), groundSprite(nullptr), birdSprite(nullptr), score(0), gameOver(false) {
    instance = this;
    srand(static_cast<unsigned int>(time(nullptr)));
}

PlayState::~PlayState() {
    destroy();
}

void PlayState::create() {
    Engine* engine = Engine::getInstance();

    backgroundSprite = new Sprite("assets/images/flappyPE/background-day.png");
    engine->addSprite(backgroundSprite);

    groundSprite = new Sprite("assets/images/flappyPE/ground.png");
    groundSprite->setPosition(0, 776);
    engine->addSprite(groundSprite);

    birdSprite = new AnimatedSprite();
    birdSprite->setPosition(100, 300);
    birdSprite->loadFrames("assets/images/flappyPE/FlappyBird.png", "assets/images/flappyPE/FlappyBird.xml");
    birdSprite->addAnimation("idle", "idle", 12, true);
    engine->addAnimatedSprite(birdSprite);
    birdSprite->playAnimation("idle");

    spawnPipe();
}

void PlayState::update(float deltaTime) {
    if (gameOver) return;

    birdSprite->update(deltaTime);
    updateBird(deltaTime);
    updatePipes(deltaTime);
    checkCollisions();

    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    }

    if (Input::getInstance().isKeyJustPressed(' ')) {
        flapBird();
    }
}

void PlayState::render() {
    backgroundSprite->render();
    
    for (auto& pipe : pipes) {
        pipe->render();
    }
    
    groundSprite->render();
    birdSprite->render();

    if (!_subStates.empty()) {
        _subStates.back()->render();
    }

    // Uncomment the line below to see collision boxes
    //renderDebug();
}

void PlayState::destroy() {
    delete backgroundSprite;
    delete groundSprite;
    delete birdSprite;
    for (auto& pipe : pipes) {
        delete pipe;
    }
    pipes.clear();
}

void PlayState::updateBird(float deltaTime) {
    float gravity = 800.0f;
    birdVelocity += gravity * deltaTime;
    birdSprite->setPosition(birdSprite->getX(), birdSprite->getY() + birdVelocity * deltaTime);

    if (birdSprite->getY() > 776 - birdSprite->getHeight()) {
        birdSprite->setPosition(birdSprite->getX(), 776 - birdSprite->getHeight());
        gameOver = true;
    }
}

void PlayState::flapBird() {
    birdVelocity = -300.0f;
}

void PlayState::spawnPipe() {
    float gapY = 150.0f + static_cast<float>(rand() % 300);
    float gapHeight = 200.0f;
    
    Sprite* topPipe = new Sprite("assets/images/flappyPE/pipe-green.png");
    topPipe->setPosition(500, gapY - 320);
    topPipe->setScale(1, -1);
    Engine::getInstance()->addSprite(topPipe);
    pipes.push_back(topPipe);

    Sprite* bottomPipe = new Sprite("assets/images/flappyPE/pipe-green.png");
    bottomPipe->setPosition(500, gapY + gapHeight);
    Engine::getInstance()->addSprite(bottomPipe);
    pipes.push_back(bottomPipe);
}

void PlayState::updatePipes(float deltaTime) {
    float pipeSpeed = 200.0f;
    
    for (auto it = pipes.begin(); it != pipes.end();) {
        (*it)->setPosition((*it)->getX() - pipeSpeed * deltaTime, (*it)->getY());
        
        if ((*it)->getX() < -52) {
            delete *it;
            it = pipes.erase(it);
        } else {
            ++it;
        }
    }

    if (pipes.empty() || pipes[pipes.size() - 1]->getX() < 250) {
        spawnPipe();
    }
}

void PlayState::checkCollisions() {
    float birdLeft = birdSprite->getX();
    float birdTop = birdSprite->getY();
    float birdRight = birdLeft + 68;
    float birdBottom = birdTop + 48;

    for (auto& pipe : pipes) {
        float pipeWidth = pipe->getWidth();
        float pipeHeight = pipe->getHeight() * std::abs(pipe->getScale().y);
        float pipeX = pipe->getX();
        float pipeY = pipe->getY() + (pipe->getScale().y < 0 ? -pipeHeight : 0);

        if (birdRight > pipeX && birdLeft < pipeX + pipeWidth &&
            birdBottom > pipeY && birdTop < pipeY + pipeHeight) {
            std::cout << "Collision detected!" << std::endl;
            gameOver = true;
            return;
        }
    }

    if (birdBottom > groundSprite->getY()) {
        std::cout << "Ground collision detected!" << std::endl;
        gameOver = true;
    }
}

void PlayState::openSubState(SubState* subState) {
    std::cout << "PlayState::openSubState called" << std::endl;
    State::openSubState(subState);
}

void PlayState::keyPressed(unsigned char key, int x, int y) {
    if (key == ' ') {
        if (gameOver) {
            restartGame();
        } else {
            flapBird();
        }
    }

    if (key == 'p') {
        if (instance->_subStates.empty()) {
            PauseSubState* pauseSubState = new PauseSubState();
            instance->openSubState(pauseSubState);
        } else {
            instance->closeSubState();
        }
    }
}

void PlayState::restartGame() {
    birdSprite->setPosition(100, 300);
    birdVelocity = 0;
    score = 0;
    gameOver = false;
    
    for (auto& pipe : pipes) {
        delete pipe;
    }
    pipes.clear();
    
    spawnPipe();
}

void PlayState::renderDebug() {
    glColor3f(1.0f, 0.0f, 0.0f);
    float birdLeft = birdSprite->getX();
    float birdTop = birdSprite->getY();
    float birdRight = birdLeft + 68;
    float birdBottom = birdTop + 48;
    
    glBegin(GL_LINE_LOOP);
    glVertex2f(birdLeft, birdTop);
    glVertex2f(birdRight, birdTop);
    glVertex2f(birdRight, birdBottom);
    glVertex2f(birdLeft, birdBottom);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    for (auto& pipe : pipes) {
        float pipeWidth = pipe->getWidth();
        float pipeHeight = pipe->getHeight() * std::abs(pipe->getScale().y);
        float pipeX = pipe->getX();
        float pipeY = pipe->getY() + (pipe->getScale().y < 0 ? -pipeHeight : 0);

        glBegin(GL_LINE_LOOP);
        glVertex2f(pipeX, pipeY);
        glVertex2f(pipeX + pipeWidth, pipeY);
        glVertex2f(pipeX + pipeWidth, pipeY + pipeHeight);
        glVertex2f(pipeX, pipeY + pipeHeight);
        glEnd();
    }
}
