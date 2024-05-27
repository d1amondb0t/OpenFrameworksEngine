#include "rectangle_drawing_tool.h"

namespace engine {

RectangleDrawingTool::RectangleDrawingTool(std::shared_ptr<DrawingCanvas> canvas) : canvas(canvas) {
    edgeWidth.set("Edge Width", 1.0f, 0.0f, 100.0f);
    fillColor.set("Fill Color", ofColor::red);
    edgeColor.set("Edge Color", ofColor::blue);
}

void RectangleDrawingTool::draw() {
    if (!isMouseLeftButtonPressed) return;

    std::unique_ptr<Rectangle> rectangle = buildRectangle();

    rectangle->draw();
}

void RectangleDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void RectangleDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mousePress = {x, y};
        mouseCurrent = {x, y};
    }
}

void RectangleDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};

        canvas->add(std::move(buildRectangle()));
    }
}

ofParameterGroup RectangleDrawingTool::getParameters() {
    return ofParameterGroup("Parameters", edgeWidth, fillColor, edgeColor);
}

std::unique_ptr<Rectangle> RectangleDrawingTool::buildRectangle() {
    glm::vec2 topLeft = {std::min(mouseCurrent.x, mousePress.x), std::min(mouseCurrent.y, mousePress.y)};
    float width = abs(mouseCurrent.x - mousePress.x);
    float height = abs(mouseCurrent.y - mousePress.y);

    return std::make_unique<Rectangle>(topLeft, width, height, edgeWidth, edgeColor, fillColor);
}

}  // namespace engine