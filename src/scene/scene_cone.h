#pragma once

#include "scene_3d_element.h"

namespace engine {

class SceneCone : public Scene3DElement {
  public:
    SceneCone(uint32_t id, std::string name, Material* material, SceneElement* parent = nullptr);

    virtual ~SceneCone();

    void draw() override;
};

}