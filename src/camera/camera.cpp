#include "camera.h"

namespace engine {

const int SPEED = 1000.f;
const int ROTATION_SPEED = 10.f;

Camera::Camera(std::shared_ptr<engine::InputController> _inputController)
    : ofCamera(), inputController(_inputController), cursorX(0), cursorY(0), deltaCursorX(0), deltaCursorY(0) {}

Camera::~Camera() {}

void Camera::update(float deltaTime, float _cursorX, float _cursorY) {
    deltaCursorX = cursorX - _cursorX;
    deltaCursorY = cursorY - _cursorY;

    cursorX = _cursorX;
    cursorY = _cursorY;

    if (inputController->isKeyPressed('w') && !inputController->isKeyPressed('s')) {
        move(getLookAtDir() * SPEED * deltaTime);
    }
    if (inputController->isKeyPressed('s') && !inputController->isKeyPressed('w')) {
        move(getLookAtDir() * -SPEED * deltaTime);
    }
    if (inputController->isKeyPressed('a') && !inputController->isKeyPressed('d')) {
        move(getXAxis() * -SPEED * deltaTime);
    }
    if (inputController->isKeyPressed('d') && !inputController->isKeyPressed('a')) {
        move(getXAxis() * SPEED * deltaTime);
    }
    if (inputController->isKeyPressed('q') && !inputController->isKeyPressed('e')) {
        move(getYAxis() * SPEED * deltaTime);
    }
    if (inputController->isKeyPressed('e') && !inputController->isKeyPressed('q')) {
        move(getYAxis() * -SPEED * deltaTime);
    }

    if (inputController->isMouseButtonPressed(OF_MOUSE_BUTTON_RIGHT)) {
        tiltDeg(ROTATION_SPEED * deltaCursorY * deltaTime);
        panDeg(ROTATION_SPEED * deltaCursorX * deltaTime);
    }
}

}  // namespace engine
