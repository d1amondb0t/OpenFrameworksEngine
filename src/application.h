#pragma once

#include <array>
#include <map>
#include <memory>

#include "ImHelpers.h"
#include "camera/camera_controller.h"
#include "graphs/histogram_wrapper.h"
#include "images/image_configuration.h"
#include "imgui_internal.h"
#include "input_controller.h"
#include "materials/material_catalog.h"
#include "ofMain.h"
#include "ofxImGui.h"
#include "renderer.h"
#include "scene/scene.h"
#include "ui/camera_window.h"
#include "ui/image_filter.h"
#include "ui/element_attribute_window.h"
#include "ui/image_capture_modal.hpp"
#include "ui/material_window.h"
#include "ui/scene_graph_window.h"
#include "ui/texture_window.h"
#include "vector_graphics/vector_draw_mode.h"
#include "window.hpp"
#include "ofxDelaunay.h"

namespace engine {
class Application : public ofBaseApp {
  public:
    Application(std::shared_ptr<Scene> scene,
                std::shared_ptr<MaterialCatalog> materialCatalog,
                std::shared_ptr<CameraController> cameraController,
                std::shared_ptr<InputController> inputController);
    ~Application();

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void exit();

  private:
    float timeCurrent;
    float timeLast;
    float timeElapsed;

    glm::vec2 currentWindowSize;

    float speed;
    float delta;
    float* valuesToChange[2];
    bool directionFactors[2];

    void prepareGui();
    void prepareToolsWindow(ofxImGui::Settings& settings);

    void onImportImageButton();
    void onExportImagesButton(CaptureSettings& settings);
    void onResetButton();
    void onToolTypeSelected(VectorDrawMode type);
    void onRefreshHistogram();

    void updateLogLevel();
    void refreshHistogram();

    bool isVerbose = false;
    bool showOrigin = false;
    VectorDrawMode selectedTool = VectorDrawMode::NONE;
    Renderer renderer;

    ofxImGui::Gui gui;
    CameraWindow cameraWindow;
    std::shared_ptr<ImageFilter> imageFilter;
    SceneGraphWindow sceneGraphWindow;
    ElementAttributeWindow elementAttributeWindow;
    MaterialWindow materialWindow;
    TextureWindow textureWindow;
    ImageCaptureModal imageCaptureModal;

    std::map<Window::Type, bool> shownWindows;
    ofParameterGroup currentDrawingToolParameters;

    GLuint histogramPixelsID;
    bool flagHistogramUpdate = false;
    HistogramWrapper histogramWrapper;

    ofFbo renderingFBO;
    ofRectangle renderingViewport;

    std::shared_ptr<InputController> inputController;

    ImageConfiguration imageConfig;

    ofxDelaunay delaunay;
    bool delaunayTriangulation = false;
};
}  // namespace engine
