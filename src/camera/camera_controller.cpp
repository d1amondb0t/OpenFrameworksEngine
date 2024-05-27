#include "camera_controller.h"

namespace engine {
CameraController::CameraController(std::shared_ptr<engine::InputController> inputController) {
    cameras = {
        {CameraType::DEFAULT, Camera(inputController)},
        {  CameraType::FRONT, Camera(inputController)},
        {   CameraType::LEFT, Camera(inputController)},
        {  CameraType::RIGHT, Camera(inputController)},
        { CameraType::BEHIND, Camera(inputController)},
        {    CameraType::TOP, Camera(inputController)},
        {  CameraType::UNDER, Camera(inputController)},
    };

    selectedCamera.set("Selected Camera", CameraType::DEFAULT, CameraType::DEFAULT, CameraType::ALL);
    selectedCamera.addListener(this, &CameraController::onSelectCamera);

    projection.set("Projection", Projection::PERSPECTIVE, Projection::PERSPECTIVE, Projection::ORTHOGRAPHIC);
    projection.addListener(this, &CameraController::onSelectProjection);

    fov.set("FOV", 60.0f, 5.0f, 175.0f);
    fov.addListener(this, &CameraController::onFovChanged);

    nearPlane.set("Near", 10.0f, 1.0f, 5000.0f);
    nearPlane.addListener(this, &CameraController::onNearChanged);

    farPlane.set("Far", 1000.0f, 1.0f, 5000.0f);
    farPlane.addListener(this, &CameraController::onFarChanged);
}

void CameraController::setup() {
    cameras.at(CameraType::DEFAULT).setPosition(0, 0, 1000.f);
    cameras.at(CameraType::FRONT).setPosition(0, 0, 1500);
    cameras.at(CameraType::LEFT).setPosition(-1500, 0, 0);
    cameras.at(CameraType::RIGHT).setPosition(1500, 0, 0);
    cameras.at(CameraType::BEHIND).setPosition(0, 0, -1500);
    cameras.at(CameraType::TOP).setPosition(0, 1500, 0);
    cameras.at(CameraType::UNDER).setPosition(0, -1500, 0);

    for (auto &cameraPair : cameras) {
        cameraPair.second.lookAt({0, 0, 0});
    }
}

void CameraController::update(float deltaTime, float cursorX, float cursorY) {
    if (activeCamNb == CameraType::DEFAULT) {
        cameras.at(CameraType::DEFAULT).update(deltaTime, cursorX, cursorY);
    }
}

std::vector<CameraType> CameraController::getViews() {
    if (activeCamNb == ALL) {
        return {FRONT, BEHIND, TOP, UNDER};
    } else {
        return {activeCamNb};
    }
}

void CameraController::endView(CameraType view) {
    cameras.at(view).end();
}

void CameraController::beginView(CameraType view, ofRectangle viewport) {
    if (activeCamNb == CameraType::ALL) {
        glm::vec2 size = {viewport.getWidth() / 2, viewport.getHeight() / 2};

        switch (view) {
            case CameraType::FRONT:
                viewport.set(viewport.getX(), viewport.getY() + size.y, size.x, size.y);
                break;

            case CameraType::BEHIND:
                viewport.set(viewport.getX() + size.x, viewport.getY() + size.y, size.x, size.y);
                break;

            case CameraType::TOP:
                viewport.set(viewport.getX(), viewport.getY(), size.x, size.y);
                break;

            case CameraType::UNDER:
                viewport.set(viewport.getX() + size.x, viewport.getY(), size.x, size.y);
                break;

            default:
                break;
        }

        ofPushStyle();
        {
            ofSetColor(0);
            ofNoFill();
            ofDrawRectangle(viewport);
        }
        ofPopStyle();
    }

    cameras.at(view).begin(viewport);
}

void CameraController::onSelectCamera(int &value) {
    activeCamNb = static_cast<CameraType>(value);
}

void CameraController::onSelectProjection(int &value) {
    for (auto &cameraPair : cameras) {
        if (value == 0) {
            cameraPair.second.disableOrtho();
        } else {
            cameraPair.second.enableOrtho();
        }
    }
}

void CameraController::onFovChanged(float &value) {
    for (auto &cameraPair : cameras) {
        cameraPair.second.setFov(value);
    }
}

void CameraController::onNearChanged(float &value) {
    for (auto &cameraPair : cameras) {
        cameraPair.second.setNearClip(value);
    }
}

void CameraController::onFarChanged(float &value) {
    for (auto &cameraPair : cameras) {
        cameraPair.second.setFarClip(value);
    }
}

}  // namespace engine
