#include "application.h"
#include "camera/camera_controller.h"
#include "input_controller.h"
#include "materials/material_catalog.h"
#include "ofMain.h"
#include "scene/scene.h"

int main() {
    // Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
    ofGLWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.setSize(1920, 1080);
    settings.windowMode = OF_WINDOW;  // can also be OF_FULLSCREEN

    auto window = ofCreateWindow(settings);

    auto inputController = std::make_shared<engine::InputController>();
    auto cameraController = std::make_shared<engine::CameraController>(inputController);
    auto scene = std::make_shared<engine::Scene>();
    auto materialCatalog = std::make_shared<engine::MaterialCatalog>();

    ofRunApp(window, make_shared<engine::Application>(scene, materialCatalog, cameraController, inputController));
    ofRunMainLoop();
}
