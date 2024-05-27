#pragma once

#include <vector>

#include "ofMain.h"

class ImageHistogram {
    static constexpr int R_COMPONENT_INDEX = 0;
    static constexpr int G_COMPONENT_INDEX = 1;
    static constexpr int B_COMPONENT_INDEX = 2;
    static constexpr int BRIGHTNESS_COUNT = 256;

  public:
    ImageHistogram();
    ImageHistogram(float px, float py, float width, float height);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    void set(float px, float py, float width, float height);
    void setPosition(float px, float py);
    void resize(float width, float height);
    void setVisible(bool isVisible);

    void load(const ofImage &image);
    void draw() const;

  private:
    void loadGrayscaleImage(const ofImage &image);
    void loadRGBImage(const ofImage &image);

    void extractRGBRectangles(std::vector<int> redFrequencies,
                              std::vector<int> greenFrequencies,
                              std::vector<int> blueFrequencies);
    void extractSingleColorRectangles(std::vector<int> frequencies, ofColor color);

    const ofImage *image;
    float x;
    float y;
    float width;
    float height;
    std::vector<ofPath> bars;
    bool isVisible;
};