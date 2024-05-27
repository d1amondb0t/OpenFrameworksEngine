#include "scene_box.h"

namespace engine {

SceneBox::SceneBox(uint32_t id, std::string name, Material* material, SceneElement* parent)
    : Scene3DElement(id, name, new ofBoxPrimitive(), material, parent) {}

SceneBox::~SceneBox() {}

void SceneBox::draw() {
    material->begin();

    node->draw();

    material->end();
}

}  // namespace engine
