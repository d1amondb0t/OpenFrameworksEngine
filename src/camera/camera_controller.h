#pragma once

#include <map>
#include <vector>

#include "camera.h"
#include "ofMain.h"

namespace engine {

enum CameraType { DEFAULT, FRONT, LEFT, RIGHT, BEHIND, TOP, UNDER, ALL };
enum Projection { PERSPECTIVE, ORTHOGRAPHIC };

class CameraController {
  public:
    CameraController(std::shared_ptr<engine::InputController> inputController);

    CameraType activeCamNb = CameraType::DEFAULT;

    void setup();
    void update(float deltaTime, float cursorX, float cursorY);

    std::vector<CameraType> getViews();
    void beginView(CameraType view, ofRectangle viewport);
    void endView(CameraType view);

    void onSelectCamera(int &value);
    void onSelectProjection(int &value);
    void onFovChanged(float &value);
    void onNearChanged(float &value);
    void onFarChanged(float &value);

    ofParameter<int> selectedCamera;
    ofParameter<int> projection;
    ofParameter<float> fov;
    ofParameter<float> nearPlane;
    ofParameter<float> farPlane;

  private:
    std::map<CameraType, Camera> cameras;
};

}  // namespace engine