#pragma once

#include "../ui/image_filter.h"
#include "../vector_graphics/image.hpp"
#include "drawing_canvas.h"
#include "drawing_tool.h"

namespace engine {

class ImageDrawingTool : public DrawingTool {
  public:
    ImageDrawingTool(std::shared_ptr<DrawingCanvas> canvas, std::shared_ptr<ImageFilter> imageFilter);

    void draw() override;
    void onMouseMoved(int x, int y) override;
    void onMousePressed(int x, int y, int button) override;
    void onMouseReleased(int x, int y, int button) override;

    void setImage(ofImage image);

    ofParameterGroup getParameters() override;

  private:
    std::unique_ptr<Image> buildImage();

    std::shared_ptr<DrawingCanvas> canvas;
    std::shared_ptr<ImageFilter> imageFilter;
    bool isMouseLeftButtonPressed = false;

    ofImage image;

    glm::vec2 mousePress = {0, 0};
    glm::vec2 mouseCurrent = {0, 0};
};

}  // namespace engine