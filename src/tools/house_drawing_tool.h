#pragma once

#include <memory>

#include "../vector_graphics/drawing_canvas.h"
#include "../vector_graphics/drawable.h"
#include "drawing_tool.h"
#include "ofMain.h"

namespace engine {
class HouseDrawingTool : public DrawingTool {
  public:
    HouseDrawingTool(std::shared_ptr<DrawingCanvas> canvas);
    void draw() override;
    void onMouseMoved(int x, int y) override;
    void onMousePressed(int x, int y, int button) override;
    void onMouseReleased(int x, int y, int button) override;

    ofParameterGroup getParameters() override;

  private:
    std::vector<std::unique_ptr<Drawable>> buildHouse();
    std::vector<std::unique_ptr<Drawable>> form;
    std::shared_ptr<DrawingCanvas> canvas;
    bool isMouseLeftButtonPressed = false;

    ofParameter<ofColor> roofColor;
    ofParameter<ofColor> roofEdgeColor;
    ofParameter<ofColor> houseColor;
    ofParameter<ofColor> houseEdgeColor;
    ofParameter<float> houseEdgeWidth;

    glm::vec2 mousePress = {0, 0};
    glm::vec2 mouseCurrent = {0, 0};
};
}  // namespace engine