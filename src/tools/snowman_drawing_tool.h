#pragma once

#include <memory>

#include "../vector_graphics/drawing_canvas.h"
#include "../vector_graphics/drawable.h"
#include "drawing_tool.h"
#include "ofMain.h"

namespace engine {
class SnowmanDrawingTool : public DrawingTool {
  public:
    SnowmanDrawingTool(std::shared_ptr<DrawingCanvas> canvas);
    void draw() override;
    void onMouseMoved(int x, int y) override;
    void onMousePressed(int x, int y, int button) override;
    void onMouseReleased(int x, int y, int button) override;

    ofParameterGroup getParameters() override;

  private:
    std::vector<std::unique_ptr<Drawable>> buildSnowman();
    std::vector<std::unique_ptr<Drawable>> form;
    std::shared_ptr<DrawingCanvas> canvas;
    bool isMouseLeftButtonPressed = false;

    ofParameter<ofColor> fillColor;
    ofParameter<ofColor> edgeColor;
    ofParameter<ofColor> noseColor;
    ofParameter<float> edgeWidth;

    glm::vec2 mousePress = {0, 0};
    glm::vec2 mouseCurrent = {0, 0};
};
}  // namespace engine