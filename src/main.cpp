#include "../include/engine/Engine.h"
#include "../include/flappyPE/states/PlayState.h"
#include <GL/glut.h>

void keyPressedCallback(unsigned char key, int x, int y) {
    if (Engine::getInstance()->getCurrentState()) {
        static_cast<PlayState*>(Engine::getInstance()->getCurrentState())->keyPressed(key, x, y);
    }
}

int main(int argc, char** argv) {
    Engine engine(499, 876, "Flappy Bird - Phlox Engine");

    PlayState* initialState = new PlayState();
    engine.pushState(initialState);

    glutKeyboardFunc(keyPressedCallback);

    engine.run();
    return 0;
}
