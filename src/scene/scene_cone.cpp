#include "scene_cone.h"

namespace engine {
SceneCone::SceneCone(uint32_t id, std::string name, Material* material, SceneElement* parent)
    : Scene3DElement(id, name, new ofConePrimitive(), material, parent) {}

SceneCone::~SceneCone() {}

void SceneCone::draw() {
    material->begin();

    node->draw();

    material->end();
}

}