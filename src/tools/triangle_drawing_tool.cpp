#include "triangle_drawing_tool.h"

namespace engine {

TriangleDrawingTool::TriangleDrawingTool(std::shared_ptr<DrawingCanvas> canvas) : canvas(canvas) {
    edgeWidth.set("Edge Width", 1.0f, 0.0f, 100.0f);
    fillColor.set("Fill Color", ofColor::red);
    edgeColor.set("Edge Color", ofColor::blue);
}

void TriangleDrawingTool::draw() {
    if (!isMouseLeftButtonPressed) return;

    std::unique_ptr<Triangle> triangle = buildTriangle();

    triangle->draw();
}

void TriangleDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void TriangleDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mousePress = {x, y};
        mouseCurrent = {x, y};
    }
}

void TriangleDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};

        canvas->add(std::move(buildTriangle()));
    }
}

ofParameterGroup TriangleDrawingTool::getParameters() {
    return ofParameterGroup("Parameters", fillColor, edgeColor);
}

std::unique_ptr<Triangle> TriangleDrawingTool::buildTriangle() {
    glm::vec2 vertex1 = mousePress;
    glm::vec2 vextex2 = {mouseCurrent.x, mousePress.y};
    glm::vec2 vertex3 = {mouseCurrent.x + ((mousePress.x - mouseCurrent.x) / 2), mouseCurrent.y};

    return std::make_unique<Triangle>(vertex1, vextex2, vertex3, edgeWidth, edgeColor, fillColor);
}

}  // namespace engine
