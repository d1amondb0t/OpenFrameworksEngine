#pragma once

#include "scene_parametric_curve.h"

namespace engine {

class SceneParametricSurface : public SceneParametricCurve {
  public:
    SceneParametricSurface(uint32_t id, std::string name, Material* material, SceneElement* parent = nullptr);
    
    virtual ~SceneParametricSurface();

    long factorial(const int n);

    int bernstein(long n, long k, long t);

    glm::vec3 blerp(double u, double v);

    glm::vec3 lerpu(double u, double v);

    glm::vec3 lerpv(double u, double v);

    glm::vec3 buildVertex(double u, double v);
    void build();

    virtual ofParameterGroup getAttributes(bool isMultiSelect) override;

    void draw() override;

  private:
    ofMesh surface;
};

}  // namespace engine
