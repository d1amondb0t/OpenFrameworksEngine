#pragma once

#include "scene_element.h"

namespace engine {

class SceneGroup : public SceneElement {
  public:
    SceneGroup(uint32_t id, std::string name, SceneElement* parent = nullptr);

    virtual ~SceneGroup();

    void draw() override;
};

}