#include "renderer.h"

#include "ray_tracer.hpp"

namespace engine {

Renderer::Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<CameraController> _cameraController)
    : scene(scene), cameraController(_cameraController) {}

Renderer::~Renderer() {}

void Renderer::setup(std::shared_ptr<engine::InputController> _inputController,
                     std::shared_ptr<ImageFilter> imageFilter) {
    inputController = _inputController;
    drawingCanvas = std::make_shared<DrawingCanvas>();
    tools = {
        {VectorDrawMode::NONE, std::make_shared<NoneDrawingTool>()},
        {VectorDrawMode::POINT, std::make_shared<PointDrawingTool>(drawingCanvas)},
        {VectorDrawMode::LINE, std::make_shared<LineDrawingTool>(drawingCanvas)},
        {VectorDrawMode::TRIANGLE, std::make_shared<TriangleDrawingTool>(drawingCanvas)},
        {VectorDrawMode::RECTANGLE, std::make_shared<RectangleDrawingTool>(drawingCanvas)},
        {VectorDrawMode::ELLIPSE, std::make_shared<EllipseDrawingTool>(drawingCanvas)},
        {VectorDrawMode::VECTOR_HOUSE, std::make_shared<HouseDrawingTool>(drawingCanvas)},
        {VectorDrawMode::VECTOR_SNOWMAN, std::make_shared<SnowmanDrawingTool>(drawingCanvas)},
        {VectorDrawMode::IMAGE, std::make_shared<ImageDrawingTool>(drawingCanvas, imageFilter)},
    };
    setDrawingTool(VectorDrawMode::NONE);

    cameraController->setup();

    cursor.setup();

    ofSetFrameRate(60);
    ofSetBackgroundColor(50);
    ofEnableDepthTest();
    ofDisableArbTex();

    locator.setPosition(0.0f, 0.0f, 0.0f);
}

void Renderer::update(float deltaTime) {
    cameraController->update(deltaTime, cursor.getMouseCurrentX(), cursor.getMouseCurrentY());
}

void Renderer::draw(ofRectangle viewport) {
    for (auto view : cameraController->getViews()) {
        cameraController->beginView(view, viewport);

        render();

        cameraController->endView(view);
    }

    cursor.draw();
    drawingCanvas->draw();
    drawingTool->draw();
}

void Renderer::render() const {
    ofEnableDepthTest();

    ofEnableLighting();

    ofSetLineWidth(2);
    ofSetColor(255);

    ofPushMatrix();

    scene->draw();

    ofDisableLighting();

    ofDisableDepthTest();

    ofPopMatrix();

    // Dessiner les axes x, y, z
    ofDisableDepthTest();
    ofSetLineWidth(4);
    ofSetColor(127);
    ofNoFill();

    drawSelectionGizmo();
    if (isOriginEnabled) {
        drawLocator(10.0f);
    }
}

void Renderer::reset() {
    drawingCanvas->reset();
}

std::shared_ptr<DrawingTool> Renderer::getDrawingTool() {
    return drawingTool;
}

std::shared_ptr<DrawingTool> Renderer::getDrawingTool(VectorDrawMode mode) {
    return tools[mode];
}

void Renderer::setDrawingTool(VectorDrawMode type) {
    auto tool = tools.find(type);
    if (tool != tools.end()) {
        drawingTool = tool->second;
    }
}

void Renderer::toggleOrigin(bool enabled) {
    isOriginEnabled = enabled;
}

void Renderer::onMouseMoved(int x, int y) {
    drawingTool->onMouseMoved(x, y);
    cursor.onMouseMoved(x, y);
}

void Renderer::onMousePressed(int x, int y, int button) {
    drawingTool->onMousePressed(x, y, button);
    cursor.onMousePressed(x, y, button);
}

void Renderer::onMouseReleased(int x, int y, int button) {
    drawingTool->onMouseReleased(x, y, button);
    cursor.onMouseReleased(x, y, button);
}

void Renderer::onKeyPressed(int key) {
    cursor.onKeyPressed(key);
    if (key == 'p') {
        renderRayTracing(scene, cameraController);
    }
}

void Renderer::onkeyReleased(int key) {
    cursor.onKeyReleased(key);
}

void Renderer::onWindowResized(int w, int h) {}

void Renderer::drawSelectionGizmo() const {
    float angle, x, y, z;
    for (SceneElement* selectedElement : scene->getSelectedElements()) {
        selectedElement->drawGizmo();
    }
}

void Renderer::drawLocator(float scale) const {
    ofPushMatrix();
    ofScale(scale, scale, scale);
    locator.draw();
    ofPopMatrix();
}

void Renderer::renderRayTracing(std::shared_ptr<Scene> scene, std::shared_ptr<CameraController> cameraController) {
    rayTracing(scene, cameraController);
}

glm::vec3 Renderer::normalizeColor(ofColor color) {
    return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f};
}

}  // namespace engine
