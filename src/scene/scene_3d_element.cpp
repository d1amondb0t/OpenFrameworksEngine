#include "scene_3d_element.h"

namespace engine {

Scene3DElement::Scene3DElement(uint32_t id, std::string name, ofNode* node, Material* material, SceneElement* parent)
    : SceneElement(id, name, node, parent), material(material) {}

Scene3DElement::~Scene3DElement() {}

const Material& Scene3DElement::getMaterial() const {
    return material == nullptr ? Material() : *material;
};

void Scene3DElement::setMaterial(Material* material) {
    this->material = material;
}

ofParameterGroup Scene3DElement::getAttributes(bool isMultiSelect) {
    ofParameterGroup parameters = SceneElement::getAttributes(isMultiSelect);

    ofParameter<string> name;
    name.set("Material: " + this->material->getName());
    parameters.add(name);

    return parameters;
}

void Scene3DElement::listen(ofParameterGroup attributesToListen, bool isMultiSelect) {
    SceneElement::listen(attributesToListen, isMultiSelect);
}

}  // namespace engine