#pragma once

#include "scene_3d_element.h"

namespace engine {

class SceneBox : public Scene3DElement {
  public:
    SceneBox(uint32_t id, std::string name, Material* material, SceneElement* parent = nullptr);

    virtual ~SceneBox();

    void draw() override;
};

}