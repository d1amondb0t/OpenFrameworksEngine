#include "image_drawing_tool.h"

namespace engine {

ImageDrawingTool::ImageDrawingTool(std::shared_ptr<DrawingCanvas> canvas, std::shared_ptr<ImageFilter> imageFilter)
    : canvas(canvas), imageFilter(imageFilter) {}

void ImageDrawingTool::setImage(ofImage image) {
    imageFilter->setDefaultImage(image);

}

ofParameterGroup ImageDrawingTool::getParameters() {
    return ofParameterGroup("Parameters");
}

void ImageDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};

        if (!image.isAllocated()) return;

        std::unique_ptr<Image> image = buildImage();

        canvas->add(std::move(image));
    }
}

void ImageDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mousePress = {x, y};
        mouseCurrent = {x, y};
        image = imageFilter->getFinalImage();
    }
}

void ImageDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void ImageDrawingTool::draw() {
    if (!isMouseLeftButtonPressed || !image.isAllocated()) return;

    std::unique_ptr<Image> image = buildImage();

    image->draw();
}

std::unique_ptr<Image> ImageDrawingTool::buildImage() {
    glm::vec2 topLeft = {std::min(mouseCurrent.x, mousePress.x), std::min(mouseCurrent.y, mousePress.y)};
    float width = abs(mouseCurrent.x - mousePress.x);
    float height = abs(mouseCurrent.y - mousePress.y);

    return std::make_unique<Image>(topLeft, width, height, image);
}

}  // namespace engine
