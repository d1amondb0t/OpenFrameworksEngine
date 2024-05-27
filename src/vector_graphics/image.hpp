#pragma once

#include "drawable.h"

namespace engine {

class Image : public Drawable {
  public:
    Image(glm::vec2 topLeftCorner, float width, float height, ofImage image)
        : topLeftCorner(topLeftCorner), width(width), height(height), image(image){};

    void draw() const override {
        ofPushStyle();
        ofSetColor(255);
        image.draw(topLeftCorner, width, height);

        ofPopStyle();
    };

  private:
    ofImage image;

    glm::vec2 topLeftCorner;
    float width;
    float height;
};
}  // namespace engine