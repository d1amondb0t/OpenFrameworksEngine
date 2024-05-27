#include "ellipse_drawing_tool.h"

namespace engine {

EllipseDrawingTool::EllipseDrawingTool(std::shared_ptr<DrawingCanvas> canvas) : canvas(canvas) {
    fillColor.set("Fill Color", ofColor::red);
    edgeColor.set("Edge Color", ofColor::blue);
    edgeWidth.set("Edge Width", 1.0f, 0.0f, 100.0f);
}

void EllipseDrawingTool::draw() {
    if (!isMouseLeftButtonPressed) return;

    std::unique_ptr<Ellipse> ellipse = buildEllipse();

    ellipse->draw();
}

void EllipseDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void EllipseDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mousePress = {x, y};
        mouseCurrent = {x, y};
    }
}

void EllipseDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};

        canvas->add(std::move(buildEllipse()));
    }
}

ofParameterGroup EllipseDrawingTool::getParameters() {
    return ofParameterGroup("Parameters", edgeWidth, fillColor, edgeColor);
}

std::unique_ptr<Ellipse> EllipseDrawingTool::buildEllipse() {
    glm::vec2 center = {mousePress.x + (mouseCurrent.x - mousePress.x) / 2,
                        mousePress.y + (mouseCurrent.y - mousePress.y) / 2};

    float width = abs(mouseCurrent.x - mousePress.x);
    float height = abs(mouseCurrent.y - mousePress.y);

    return std::make_unique<Ellipse>(center, width, height, edgeWidth, edgeColor, fillColor);
}

}  // namespace engine