#pragma once

#include "scene_element.h"

namespace engine {

class SceneLight : public SceneElement {
  public:
    SceneLight(uint32_t id, std::string name, SceneElement* parent = nullptr);

    void draw() override;

    virtual ofParameterGroup getAttributes(bool isMultiSelect) override;
    virtual void listen(ofParameterGroup attributesToListen, bool isMultiselect) override;

  private:
    void onToggleEnabled(bool& enabled);
    void onPositionChanged(glm::vec3& pos);
    void onAmbientColorChanged(ofFloatColor& color);
    void onDiffuseColorChanged(ofFloatColor& color);
    void onSpecularColorChanged(ofFloatColor& color);

    ofLight light;
};

}  // namespace engine