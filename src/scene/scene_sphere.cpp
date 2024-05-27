#include "scene_sphere.h"

namespace engine {

SceneSphere::SceneSphere(uint32_t id, std::string name, Material* material, SceneElement* parent)
    : Scene3DElement(id, name, new ofSpherePrimitive(), material, parent) {}

SceneSphere::~SceneSphere() {}

void SceneSphere::draw() {
    material->begin();

    node->draw();

    material->end();
}

}  // namespace engine