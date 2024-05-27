#pragma once

#include "ofMain.h"

namespace engine {

class DrawingTool {
  public:
    virtual void draw() = 0;
    virtual void onMouseMoved(int x, int y) = 0;
    virtual void onMousePressed(int x, int y, int button) = 0;
    virtual void onMouseReleased(int x, int y, int button) = 0;

    virtual ofParameterGroup getParameters() = 0;
};

}  // namespace engine
