#include "scene_plane.h"

engine::ScenePlane::ScenePlane(uint32_t id, std::string name, Material* material, SceneElement* parent)
    : Scene3DElement(id, name, new ofPlanePrimitive(), material, parent) {}

engine::ScenePlane::~ScenePlane() {}

void engine::ScenePlane::draw() {
    node->draw();
}
