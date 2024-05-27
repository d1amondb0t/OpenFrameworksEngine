#include "scene_3d_element.h"

namespace engine {

class ScenePlane : public Scene3DElement {
  public:
    ScenePlane(uint32_t id, std::string name, Material* material, SceneElement* parent = nullptr);

    virtual ~ScenePlane();

    void draw() override;

  private:
};

}  // namespace engine