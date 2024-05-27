#include "camera_window.h"

namespace engine {

CameraWindow::CameraWindow(std::shared_ptr<CameraController> cameraController) : cameraController(cameraController) {}

void CameraWindow::prepare(ofxImGui::Settings& settings) {
    ofxImGui::BeginWindow("Camera", settings);

    ofxImGui::AddRadio(cameraController->selectedCamera, {"Default", "Front", "Left", "Right", "Behind", "Top", "Under", "4 views"});
    ofxImGui::AddRadio(cameraController->projection, {"Perspective", "Orthogonale"});

    ofxImGui::AddSlider(cameraController->fov);
    ofxImGui::AddSlider(cameraController->nearPlane, "%.3f", true);
    ofxImGui::AddSlider(cameraController->farPlane, "%.3f", true);

    ofxImGui::EndWindow(settings);
}

}  // namespace engine
 