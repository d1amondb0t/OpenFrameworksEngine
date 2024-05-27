#pragma once

#include "scene_element.h"

namespace engine {

class Scene3DElement : public SceneElement {
  public:
    Scene3DElement(uint32_t id, std::string name, ofNode* node, Material* material, SceneElement* parent = nullptr);

    virtual ~Scene3DElement();

    virtual const Material& getMaterial() const override;
    virtual void setMaterial(Material* material) override;

  protected:
    Material* material;

  public:
    virtual ofParameterGroup getAttributes(bool isMultiSelect) override;
    virtual void listen(ofParameterGroup attributesToListen, bool isMultiSelect) override;
};

}