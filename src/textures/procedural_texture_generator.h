#pragma once

#include "ofMain.h"

class ProceduralTextureGenerator {
  public:
    ProceduralTextureGenerator(int textureSize);

    void generateCheckerTexture(ofTexture& target, int blockCount, ofColor evenColor, ofColor oddColor);
    void generateBricksTexture(
        ofTexture& target, int brickWidth, int brickHeight, int brickPadding, ofColor brickColor, ofColor marginColor);
    void generatePerlinNoiseTexture(
        ofTexture& target, float frequency, float midpoint, float mixEdgeSize, ofColor highColor, ofColor lowColor);

  private:
    int textureSize;
};