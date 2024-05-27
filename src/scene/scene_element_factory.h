#pragma once

#include "../materials/material_catalog.h"
#include "ofMain.h"
#include "scene_element.h"
#include "scene_group.h"
#include "scene_light.h"
#include "scene_parametric_curve.h"
#include "scene_parametric_surface.h"
#include "scene_box.h"
#include "scene_sphere.h"
#include "scene_plane.h"
#include "scene_cone.h"
#include "scene_3d_model.h"

namespace engine {

class SceneElementFactory {
    static uint32_t nbInstances;

  public:
    SceneElementFactory(std::shared_ptr<MaterialCatalog> materialCatalog);
    ~SceneElementFactory();

    SceneGroup* createSceneGroup(SceneElement* parent = nullptr) const;
    SceneLight* createSceneLight(SceneElement* parent = nullptr) const;
    SceneParametricCurve* createSceneParametricCurve(SceneElement* parent = nullptr) const;
    SceneParametricSurface* createSceneParametricSurface(SceneElement* parent = nullptr) const;
    SceneBox* createSceneBox(SceneElement* parent = nullptr) const;
    SceneSphere* createSceneSphere(SceneElement* parent = nullptr) const;
    SceneCone* createSceneCone(SceneElement* parent = nullptr) const;
    ScenePlane* createScenePlane(SceneElement* parent = nullptr) const;
    Scene3DModel* createScene3dModel(std::string path,
                                     std::string materialName = "",
                                     SceneElement* parent = nullptr) const;

  private:
    std::shared_ptr<MaterialCatalog> materialCatalog;
    std::string name;
};

}  // namespace engine