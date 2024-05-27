#include "scene_element_factory.h"

namespace engine {

uint32_t SceneElementFactory::nbInstances = 0;

SceneElementFactory::SceneElementFactory(std::shared_ptr<MaterialCatalog> materialCatalog)
    : materialCatalog(materialCatalog) {}

SceneElementFactory::~SceneElementFactory() {}

SceneGroup* SceneElementFactory::createSceneGroup(SceneElement* parent) const {
    return new SceneGroup(++nbInstances, "Group", parent);
}

SceneLight* SceneElementFactory::createSceneLight(SceneElement* parent) const {
    return new SceneLight(++nbInstances, "Light", parent);
}

SceneParametricCurve* SceneElementFactory::createSceneParametricCurve(SceneElement* parent) const {
    return new SceneParametricCurve(++nbInstances, "Curve", materialCatalog->getMaterial(0), parent);
}

SceneParametricSurface* SceneElementFactory::createSceneParametricSurface(SceneElement* parent) const {
    return new SceneParametricSurface(++nbInstances, "Surface", materialCatalog->getMaterial(0), parent);
}

SceneBox* SceneElementFactory::createSceneBox(SceneElement* parent) const {
    return new SceneBox(++nbInstances, "Box", materialCatalog->getMaterial(0), parent);
}

SceneSphere* SceneElementFactory::createSceneSphere(SceneElement* parent) const {
    return new SceneSphere(++nbInstances, "Sphere", materialCatalog->getMaterial(0), parent);
}

SceneCone* SceneElementFactory::createSceneCone(SceneElement* parent) const {
    return new SceneCone(++nbInstances, "Cone", materialCatalog->getMaterial(0), parent);
}

Scene3DModel* SceneElementFactory::createScene3dModel(std::string path,
                                                      std::string materialName,
                                                      SceneElement* parent) const {
    Material* material = materialCatalog->getMaterial(materialName);
    if (material == nullptr) {
        material = materialCatalog->getMaterial(0);
    }

    return new Scene3DModel(++nbInstances, "3dObject", path, material, parent);
}

ScenePlane* SceneElementFactory::createScenePlane(SceneElement* parent) const {
    return new ScenePlane(++nbInstances, "Plane", materialCatalog->getMaterial(0), parent);
}

}  // namespace engine
