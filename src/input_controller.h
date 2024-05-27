#pragma once

#include <map>

#include "ofMain.h"

namespace engine {

class InputController {
  public:
    InputController();

    bool isKeyPressed(int key);
    void pressKey(int key);
    void releaseKey(int key);

    bool isMouseButtonPressed(int button);
    void pressMouseButton(int button);
    void releaseMouseButton(int button);

  private:
    bool keyExists(int key);
    bool mouseButtonExists(int button);

    std::map<int, bool> mouseButtonsPressed;
    std::map<int, bool> keysPressed;
};

}  // namespace engine
