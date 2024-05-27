#pragma once

#include "ImHelpers.h"
#include "../camera/camera_controller.h"
#include "ofMain.h"
#include "ofxImGui.h"

namespace engine {

class CameraWindow {
  public:
    CameraWindow(std::shared_ptr<CameraController> cameraController);

    void prepare(ofxImGui::Settings& settings);

  private:
    std::shared_ptr<CameraController> cameraController;
};

}  // namespace engine
