#include "house_drawing_tool.h"

#include "../vector_graphics/rectangle.hpp"
#include "../vector_graphics/triangle.hpp"

namespace engine {

HouseDrawingTool::HouseDrawingTool(std::shared_ptr<DrawingCanvas> canvas) : canvas(canvas) {
    roofColor.set("Roof Color", ofColor::rosyBrown);
    roofEdgeColor.set("Roof Edge Color", ofColor::sandyBrown);
    houseColor.set("House Color", ofColor::brown);
    houseEdgeColor.set("House Edge Color", ofColor::white);
    houseEdgeWidth.set("House Edge Width", 1.0f, 0.0f, 100.0f);
}

void HouseDrawingTool::draw() {
    if (!isMouseLeftButtonPressed) return;

    form = buildHouse();
    for (const auto& primitive : form) {
        primitive->draw();
    }
}

void HouseDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void HouseDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mousePress = {x, y};
        mouseCurrent = {x, y};
    }
}

void HouseDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};
        for (auto& shape : buildHouse()) {
            canvas->add(std::move(shape));
        }
    }
}

ofParameterGroup HouseDrawingTool::getParameters() {
    return ofParameterGroup("Parameters", roofColor, roofEdgeColor, houseColor, houseEdgeColor, houseEdgeWidth);
}

std::vector<std::unique_ptr<Drawable>> HouseDrawingTool::buildHouse() {
    glm::vec2 topRight = {std::min(mousePress.x, mouseCurrent.x), std::min(mousePress.y, mouseCurrent.y)};
    float width = abs(mouseCurrent.x - mousePress.x);
    float height = abs(mouseCurrent.y - mousePress.y);

    glm::vec2 rectangleTopRight = {topRight.x, topRight.y + height / 2};

    glm::vec2 vertex1 = {topRight.x, topRight.y + height / 2};
    glm::vec2 vertex2 = {topRight.x + width / 2, topRight.y};
    glm::vec2 vertex3 = {topRight.x + width, topRight.y + height / 2};

    std::vector<std::unique_ptr<Drawable>> tempForm;

    auto t = std::make_unique<Triangle>(vertex1, vertex2, vertex3, 1, roofEdgeColor, roofColor);
    auto r =
        std::make_unique<Rectangle>(rectangleTopRight, width, height / 2, houseEdgeWidth, houseEdgeColor, houseColor);

    tempForm.push_back(std::move(t));
    tempForm.push_back(std::move(r));
    return tempForm;
}

}  // namespace engine