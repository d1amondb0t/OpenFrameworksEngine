#include "procedural_texture_generator.h"

ProceduralTextureGenerator::ProceduralTextureGenerator(int textureSize) : textureSize(textureSize) {}

void ProceduralTextureGenerator::generateCheckerTexture(ofTexture& target,
                                                        int blockCount,
                                                        ofColor evenColor,
                                                        ofColor oddColor) {
    ofPixels pixels;
    pixels.allocate(textureSize, textureSize, OF_PIXELS_RGBA);

    int blockSize = textureSize / blockCount;

    for (size_t i = 0; i < textureSize; i++) {
        for (size_t j = 0; j < textureSize; j++) {
            int t = (i / blockSize) + (j / blockSize);

            pixels.setColor(i, j, t % 2 == 0 ? evenColor : oddColor);
        }
    }

    target.loadData(pixels);
}

void ProceduralTextureGenerator::generateBricksTexture(
    ofTexture& target, int brickWidth, int brickHeight, int brickPadding, ofColor brickColor, ofColor marginColor) {
    ofPixels pixels;
    pixels.allocate(textureSize, textureSize, OF_PIXELS_RGBA);

    int brickRowOffset = brickWidth / 2;

    for (size_t i = 0; i < textureSize; i++) {
        for (size_t j = 0; j < textureSize; j++) {
            int currentBrickRow = j / (brickHeight + brickPadding);
            int offset = (currentBrickRow % 2) * brickRowOffset;

            bool isBrick = (i + offset) % (brickWidth + brickPadding) <= brickWidth &&
                           j % (brickHeight + brickPadding) <= brickHeight;

            pixels.setColor(i, j, isBrick ? brickColor : marginColor);
        }
    }

    target.loadData(pixels);
}

void ProceduralTextureGenerator::generatePerlinNoiseTexture(
    ofTexture& target, float frequency, float midpoint, float mixEdgeSize, ofColor highColor, ofColor lowColor) {
    ofPixels pixels;
    pixels.allocate(textureSize, textureSize, OF_PIXELS_RGBA);

    for (size_t i = 0; i < textureSize; i++) {
        for (size_t j = 0; j < textureSize; j++) {
            float f_i = i;
            float f_j = j;

            float n = ofNoise(f_i * frequency / textureSize, f_j * frequency / textureSize);
            float diff = midpoint - n;

            ofColor o;
            if (diff >= mixEdgeSize / 2) {
                o = highColor;
            } else if (diff <= -mixEdgeSize / 2) {
                o = lowColor;
            } else {
                o = highColor / 2 + lowColor / 2;
            }

            pixels.setColor(i, j, o);
        }
    }

    target.loadData(pixels);
}
