#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "scene_3d_element.h"

namespace engine {

class Scene3DModel : public Scene3DElement {
  public:
    Scene3DModel(uint32_t id, std::string name, const std::string modelPath, Material* material, SceneElement* parent = nullptr);

    virtual ~Scene3DModel();

    void draw() override;

  private:
    void loadModelTangents();
    float* tangents;
    float* bitangents;

    ofxAssimpModelLoader model;
};

}  // namespace engine