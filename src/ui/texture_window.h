#pragma once

#include <memory>
#include <vector>

#include "../textures/procedural_texture_generator.h"
#include "ImHelpers.h"
#include "ofxImGui.h"
#include "texture_payload.hpp"

namespace engine {

enum ProceduralGenerationType { CHECKERS = 0, BRICKS = 1, NOISE = 2 };

class TextureWindow {
  public:
    TextureWindow();

    void prepare(ofxImGui::Settings& settings);

  private:
    void generateTexture();

    std::vector<std::string> generationTypeLabels;
    ofParameter<int> proceduralGenerationType;

    ofParameter<int> checkersCount;
    ofParameter<ofColor> evenCheckersColor;
    ofParameter<ofColor> oddCheckersColor;

    ofParameter<int> bricksWidth;
    ofParameter<int> bricksHeight;
    ofParameter<int> marginSize;
    ofParameter<ofColor> brickColor;
    ofParameter<ofColor> marginColor;

    ofParameter<float> frequency;
    ofParameter<float> midpoint;
    ofParameter<float> mixEdgeSize;
    ofParameter<ofColor> highValuesColor;
    ofParameter<ofColor> lowValuesColor;

    std::vector<std::shared_ptr<ofTexture>> textures;
    ProceduralTextureGenerator textureGenerator;
};

}  // namespace engine
