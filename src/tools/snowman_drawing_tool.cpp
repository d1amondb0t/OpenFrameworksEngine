#include "snowman_drawing_tool.h"

#include "../vector_graphics/ellipse.hpp"
#include "../vector_graphics/triangle.hpp"

namespace engine {

SnowmanDrawingTool::SnowmanDrawingTool(std::shared_ptr<DrawingCanvas> canvas) : canvas(canvas) {
    fillColor.set("Fill Color", ofColor::white);
    edgeColor.set("Edge Color", ofColor::black);
    edgeWidth.set("Edge Width", 1.0f, 0.0f, 100.0f);
    noseColor.set("Nose Color", ofColor::orange);
}

void SnowmanDrawingTool::draw() {
    if (!isMouseLeftButtonPressed) return;

    form = buildSnowman();
    for (const auto& primitive : form) {
        primitive->draw();
    }
}

void SnowmanDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void SnowmanDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mousePress = {x, y};
        mouseCurrent = {x, y};
    }
}

void SnowmanDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};

        for (auto& shape : buildSnowman()) {
            canvas->add(std::move(shape));
        }
    }
}

ofParameterGroup SnowmanDrawingTool::getParameters() {
    return ofParameterGroup("Parameters", edgeWidth, fillColor, edgeColor, noseColor);
}

std::vector<std::unique_ptr<Drawable>> SnowmanDrawingTool::buildSnowman() {
    glm::vec2 topLeft = {std::min(mousePress.x, mouseCurrent.x), std::min(mousePress.y, mouseCurrent.y)};
    glm::vec2 bottomRight = {std::max(mousePress.x, mouseCurrent.x), std::max(mousePress.y, mouseCurrent.y)};

    float width = abs(topLeft.x - bottomRight.x);
    float height = abs(topLeft.y - bottomRight.y) / 6;

    glm::vec2 topCirclePosition = {topLeft.x + width / 2, topLeft.y + height};
    glm::vec2 bottomCirclePosition = {topLeft.x + width / 2, topLeft.y + 4 * height};

    std::vector<std::unique_ptr<Drawable>> tempForm;

    auto topCircle =
        std::make_unique<Ellipse>(topCirclePosition, width / 2, 2 * height, edgeWidth, edgeColor, fillColor);
    auto bottomCircle =
        std::make_unique<Ellipse>(bottomCirclePosition, width, 4 * height, edgeWidth, edgeColor, fillColor);

    float nose_proportion = 0.05f;
    glm::vec2 topVertex = {topLeft.x + width / 2, topLeft.y + height};
    glm::vec2 bottomLeftVertex = {topVertex.x - width * nose_proportion, topVertex.y + height * 2 * nose_proportion};
    glm::vec2 bottomRightVertex = {topVertex.x + width * nose_proportion, topVertex.y + height * 2 * nose_proportion};
    auto nose = std::make_unique<Triangle>(topVertex, bottomLeftVertex, bottomRightVertex, 1, edgeColor, noseColor);

    tempForm.push_back(std::move(topCircle));
    tempForm.push_back(std::move(bottomCircle));
    tempForm.push_back(std::move(nose));
    return tempForm;
}

}  // namespace engine