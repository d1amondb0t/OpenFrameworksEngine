#pragma once

#include "scene_3d_element.h"

namespace engine {

class SceneSphere : public Scene3DElement {
  public:
    SceneSphere(uint32_t id, std::string name, Material* material, SceneElement* parent = nullptr);

    ~SceneSphere();

    void draw() override;
};

}