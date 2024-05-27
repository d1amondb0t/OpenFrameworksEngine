#pragma once

#include <map>
#include <memory>

#include "ofMain.h"

namespace engine {

class Material : public ofMaterial {
  public:
    Material(std::string name = "");

    std::string getName();
    void setName(std::string name);

    bool isToon();
    void setIsToon(bool enabled);

    float getRimAmount();
    void setRimAmount(float amount);

    float getRimThreshold();
    void setRimThreshold(float amount);

    ofFloatColor getRimColor();
    void setRimColor(ofFloatColor color);

    ofParameterGroup getAttributes();

    void loadTexture(const ofMaterialTextureType& aMaterialTextureType, const std::string path);
    void resetTextures();
    void setTexture(const ofMaterialTextureType& aMaterialTextureType, const ofTexture& aTex);
    bool removeCustomUniformTexture(const ofMaterialTextureType& aMaterialTextureType);
    bool removeCustomUniformTexture(const std::string& name);

  private:
    void onNameChanged(std::string& name);
    void onAmbientColorChanged(ofFloatColor& color);
    void onDiffuseColorChanged(ofFloatColor& color);
    void onSpecularColorChanged(ofFloatColor& color);
    void onShininessChanged(float& shininess);
    void onMetallicChanged(float& metallic);
    void onRoughnessChanged(float& roughness);

    void onRimAmountChanged(float& rimAmount);
    void onRimThresholdChanged(float& rimThreshold);
    void onRimColorChanged(ofFloatColor& color);

    std::string name;
    std::map<ofMaterialTextureType, std::shared_ptr<ofTexture>> textures;

    // Toon parameters
    bool isToonEnabled;
    float rimAmount;
    float rimThreshold;
    ofFloatColor rimColor;
};

}  // namespace engine