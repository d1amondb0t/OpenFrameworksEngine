#pragma once

#include "drawing_tool.h"
#include "ofMain.h"

namespace engine {

class NoneDrawingTool : public DrawingTool {
    void draw() override{};
    void onMouseMoved(int x, int y) override{};
    void onMousePressed(int x, int y, int button) override{};
    void onMouseReleased(int x, int y, int button) override{};

    ofParameterGroup getParameters() override { return ofParameterGroup("Parameters"); };
};

}  // namespace engine