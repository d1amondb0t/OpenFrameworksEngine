#pragma once

#include <map>
#include <memory>

#include "camera/camera_controller.h"
#include "ofMain.h"
#include "scene/scene.h"
#include "scene/scene_3d_model.h"
#include "scene/scene_element_factory.h"
#include "tools/drawing_tool.h"
#include "tools/ellipse_drawing_tool.h"
#include "tools/house_drawing_tool.h"
#include "tools/image_drawing_tool.h"
#include "tools/line_drawing_tool.h"
#include "tools/none_drawing_tool.hpp"
#include "tools/point_drawing_tool.h"
#include "tools/rectangle_drawing_tool.h"
#include "tools/snowman_drawing_tool.h"
#include "tools/triangle_drawing_tool.h"
#include "ui/cursor.h"
#include "ui/image_filter.h"
#include "vector_graphics/drawing_canvas.h"
#include "vector_graphics/vector_draw_mode.h"

namespace engine {

class Renderer {
  public:
    Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<CameraController> cameraController);
    ~Renderer();

    void setup(std::shared_ptr<engine::InputController> _inputController, std::shared_ptr<ImageFilter> imageFilter);
    void reset();
    void update(float deltaTime);
    void draw(ofRectangle viewport);

    std::shared_ptr<DrawingTool> getDrawingTool();
    std::shared_ptr<DrawingTool> getDrawingTool(VectorDrawMode mode);
    void setDrawingTool(VectorDrawMode type);

    void toggleOrigin(bool enabled);

    void onMouseMoved(int x, int y);
    void onMousePressed(int x, int y, int button);
    void onMouseReleased(int x, int y, int button);

    void onKeyPressed(int key);
    void onkeyReleased(int key);

    void onWindowResized(int w, int h);

  private:
    std::shared_ptr<Scene> scene;
    std::shared_ptr<DrawingTool> drawingTool;
    std::shared_ptr<DrawingCanvas> drawingCanvas;
    std::map<VectorDrawMode, std::shared_ptr<DrawingTool>> tools;

    ofNode locator;

    Cursor cursor;

    std::shared_ptr<InputController> inputController;
    std::shared_ptr<CameraController> cameraController;

    bool isOriginEnabled;

    void drawSelectionGizmo() const;
    void drawLocator(float scale) const;
    void render() const;

    void renderRayTracing(std::shared_ptr<Scene> scene, std::shared_ptr<CameraController> cameraController);
    glm::vec3 normalizeColor(ofColor color);
};
}  // namespace engine
