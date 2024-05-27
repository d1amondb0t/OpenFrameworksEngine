#include "scene_light.h"

namespace engine {
SceneLight::SceneLight(uint32_t id, std::string name, SceneElement* parent)
    : SceneElement(id, name, new ofNode(), parent) {
    light.setParent(*this->node);
    light.setPointLight();

    light.enable();
}

void SceneLight::draw() {
    if (light.getIsEnabled()) {
        light.draw();
    }
}

ofParameterGroup SceneLight::getAttributes(bool isMultiSelect) {
    ofParameterGroup parameters("Attributes");

    ofParameter<bool> enabled;
    enabled.set("Enabled", light.getIsEnabled());
    parameters.add(enabled);

    ofParameter<glm::vec3> position;
    position.set("Position", light.getGlobalPosition(), glm::vec3(-5000), glm::vec3(5000));
    parameters.add(position);

    ofParameter<ofFloatColor> ambientColor = light.getAmbientColor();
    ambientColor.setName("Ambient Color");
    ambientColor.addListener(this, &SceneLight::onAmbientColorChanged);
    parameters.add(ambientColor);

    ofParameter<ofFloatColor> diffuseColor = light.getDiffuseColor();
    diffuseColor.setName("Diffuse Color");
    diffuseColor.addListener(this, &SceneLight::onDiffuseColorChanged);
    parameters.add(diffuseColor);

    ofParameter<ofFloatColor> specularColor = light.getSpecularColor();
    specularColor.setName("Specular Color");
    specularColor.addListener(this, &SceneLight::onSpecularColorChanged);
    parameters.add(specularColor);

    return parameters;
}

void SceneLight::listen(ofParameterGroup attributesToListen, bool isMultiselect) {
    SceneElement::listen(attributesToListen, isMultiselect);
    string attrName;
    for (auto attr : attributesToListen) {
        attrName = attr->getName();
        if (attrName == "Enabled") {
            attr->cast<bool>().addListener(this, &SceneLight::onToggleEnabled);
        } else if (attrName == "Position") {
            attr->cast<glm::vec3>().addListener(this, &SceneLight::onPositionChanged);
        } else if (attrName == "Ambient Color") {
            attr->cast<ofFloatColor>().addListener(this, &SceneLight::onAmbientColorChanged);
        } else if (attrName == "Diffuse Color") {
            attr->cast<ofFloatColor>().addListener(this, &SceneLight::onDiffuseColorChanged);
        } else if (attrName == "Specular Color") {
            attr->cast<ofFloatColor>().addListener(this, &SceneLight::onSpecularColorChanged);
        }
    }
}

void SceneLight::onToggleEnabled(bool& enabled) {
    if (enabled) {
        light.enable();
    } else {
        light.disable();
    }
}

void SceneLight::onPositionChanged(glm::vec3& pos) {
    light.setGlobalPosition(pos);
}

void SceneLight::onAmbientColorChanged(ofFloatColor& color) {
    light.setAmbientColor(color);
}

void SceneLight::onDiffuseColorChanged(ofFloatColor& color) {
    light.setDiffuseColor(color);
}

void SceneLight::onSpecularColorChanged(ofFloatColor& color) {
    light.setSpecularColor(color);
}

}  // namespace engine
