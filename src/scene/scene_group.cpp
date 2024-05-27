#include "scene_group.h"

namespace engine {

SceneGroup::SceneGroup(uint32_t id, std::string name, SceneElement* parent)
    : SceneElement(id, name, new ofNode(), parent) {}

SceneGroup::~SceneGroup() {}

void SceneGroup::draw() {}

}  // namespace engine