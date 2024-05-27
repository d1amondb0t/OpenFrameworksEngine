#pragma once

#include <memory>

#include "../vector_graphics/drawing_canvas.h"
#include "../vector_graphics/point.hpp"
#include "drawing_tool.h"
#include "ofMain.h"

namespace engine {

class PointDrawingTool : public DrawingTool {
  public:
    PointDrawingTool(std::shared_ptr<DrawingCanvas> canvas);

    void draw() override;
    void onMouseMoved(int x, int y) override;
    void onMousePressed(int x, int y, int button) override;
    void onMouseReleased(int x, int y, int button) override;

    ofParameterGroup getParameters() override;

  private:
    std::unique_ptr<Point> buildPoint();

    std::shared_ptr<DrawingCanvas> canvas;
    bool isMouseLeftButtonPressed = false;

    ofParameter<ofColor> color = ofColor::red;
    ofParameter<float> radius = 1;

    glm::vec2 mouseCurrent = {0, 0};
};

}  // namespace engine