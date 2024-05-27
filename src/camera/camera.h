#pragma once
#include <memory>

#include "input_controller.h"
#include "ofCamera.h"

namespace engine {
class Camera : public ofCamera {
  public:
    Camera(std::shared_ptr<engine::InputController> _inputController);
    ~Camera();

    void update(float deltaTime, float _cursorX, float _cursorY);

    std::shared_ptr<engine::InputController> inputController;

  private:
    float cursorX;
    float cursorY;
    float deltaCursorX;
    float deltaCursorY;
};

}  // namespace engine