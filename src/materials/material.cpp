#include "material.h"

namespace engine {

Material::Material(std::string name) : ofMaterial(), name(name), isToonEnabled(false) {
    setRimAmount(0.716);
    setRimThreshold(0.1);
    setRimColor(ofFloatColor::white);
}

std::string Material::getName() {
    return name;
}

void Material::setName(std::string name) {
    this->name = name;
}

bool Material::isToon() {
    return isToonEnabled;
}

void Material::setIsToon(bool enabled) {
    isToonEnabled = enabled;
}

float Material::getRimAmount() {
    return rimAmount;
}

void Material::setRimAmount(float amount) {
    rimAmount = amount;

    setCustomUniform1f("rim_amount", rimAmount);
}

float Material::getRimThreshold() {
    return rimThreshold;
}

void Material::setRimThreshold(float amount) {
    rimThreshold = amount;

    setCustomUniform1f("rim_threshold", rimThreshold);
}

ofFloatColor Material::getRimColor() {
    return rimColor;
}

void Material::setRimColor(ofFloatColor color) {
    rimColor = color;

    setCustomUniform4f("mat_rim", {rimColor.r, rimColor.g, rimColor.b, rimColor.a});
}

ofParameterGroup Material::getAttributes() {
    ofParameterGroup attributes("Attributes");

    ofParameter<string> name;
    name.set("Name", getName());
    name.addListener(this, &Material::onNameChanged);
    attributes.add(name);

    ofParameter<ofFloatColor> ambientColor;
    ambientColor.set("Ambient color", getAmbientColor());
    ambientColor.addListener(this, &Material::onAmbientColorChanged);
    attributes.add(ambientColor);

    ofParameter<ofFloatColor> diffuseColor;
    diffuseColor.set("Diffuse color", getDiffuseColor());
    diffuseColor.addListener(this, &Material::onDiffuseColorChanged);
    attributes.add(diffuseColor);

    ofParameter<ofFloatColor> specularColor;
    specularColor.set("Specular color", getSpecularColor());
    specularColor.addListener(this, &Material::onSpecularColorChanged);
    attributes.add(specularColor);

    ofParameter<float> shininess;
    shininess.set("Shininess", getShininess(), 0, 4096);
    shininess.addListener(this, &Material::onShininessChanged);
    attributes.add(shininess);

    if (isPBR()) {
        ofParameter<float> metallic;
        metallic.set("Metallic", getMetallic());
        metallic.addListener(this, &Material::onMetallicChanged);
        attributes.add(metallic);

        ofParameter<float> roughness;
        roughness.set("Roughness", getRoughness());
        roughness.addListener(this, &Material::onRoughnessChanged);
        attributes.add(roughness);
    }

    if (isToonEnabled) {
        ofParameter<float> rimAmount;
        rimAmount.set("Rim Amount", getRimAmount());
        rimAmount.addListener(this, &Material::onRimAmountChanged);
        attributes.add(rimAmount);

        ofParameter<float> rimThreshold;
        rimThreshold.set("Rim Threshold", getRimThreshold());
        rimThreshold.addListener(this, &Material::onRimThresholdChanged);
        attributes.add(rimThreshold);

        ofParameter<ofFloatColor> rimColor;
        rimColor.set("Rim Color", getRimColor());
        rimColor.addListener(this, &Material::onRimColorChanged);
        attributes.add(rimColor);
    }

    return attributes;
}

void Material::loadTexture(const ofMaterialTextureType& aMaterialTextureType, const std::string path) {
    removeCustomUniformTexture(aMaterialTextureType);

    std::shared_ptr<ofTexture> texture = std::make_shared<ofTexture>();
    textures.insert({aMaterialTextureType, texture});
    ofLoadImage(*texture, path);

    setTexture(aMaterialTextureType, *texture);
}

void Material::resetTextures() {
    for (int i = OF_MATERIAL_TEXTURE_NONE + 1; i < OF_MATERIAL_TEXTURE_CLEARCOAT_NORMAL; ++i) {
        auto type = static_cast<ofMaterialTextureType>(i);
        removeCustomUniformTexture(type);
    }
}

void Material::setTexture(const ofMaterialTextureType& aMaterialTextureType, const ofTexture& aTex) {
    std::string textureName = getUniformName(aMaterialTextureType);
    setCustomUniform1i("has_" + textureName, 1);

    ofMaterial::setTexture(aMaterialTextureType, aTex);
}

bool Material::removeCustomUniformTexture(const ofMaterialTextureType& aMaterialTextureType) {
    textures.erase(aMaterialTextureType);

    return removeCustomUniformTexture(getUniformName(aMaterialTextureType));
}

bool Material::removeCustomUniformTexture(const std::string& name) {
    setCustomUniform1i("has_" + name, 0);

    return ofMaterial::removeCustomUniformTexture(name);
}

void Material::onNameChanged(std::string& name) {
    setName(name);
}

void Material::onAmbientColorChanged(ofFloatColor& color) {
    setAmbientColor(color);
}

void Material::onDiffuseColorChanged(ofFloatColor& color) {
    setDiffuseColor(color);
}

void Material::onSpecularColorChanged(ofFloatColor& color) {
    setSpecularColor(color);
}

void Material::onShininessChanged(float& shininess) {
    setShininess(shininess);
}

void Material::onMetallicChanged(float& metallic) {
    setMetallic(metallic);
}

void Material::onRoughnessChanged(float& roughness) {
    setRoughness(roughness);
}

void Material::onRimAmountChanged(float& rimAmount) {
    setRimAmount(rimAmount);
}

void Material::onRimThresholdChanged(float& rimThreshold) {
    setRimThreshold(rimThreshold);
}

void Material::onRimColorChanged(ofFloatColor& color) {
    setRimColor(color);
}

}  // namespace engine