#pragma once

#include "ofMain.h"
#include "drawable.h"

namespace engine {
class Ellipse : public Drawable {
  public:
    Ellipse(glm::vec2 position, float width, float height, float edgeWidth, ofColor edgeColor, ofColor fillColor)
        : Drawable(),
          position(position),
          width(width),
          height(height),
          edgeWidth(edgeWidth),
          edgeColor(edgeColor),
          fillColor(fillColor) {}

    void draw() const override {
        ofFill();
        ofSetColor(edgeColor);
        ofDrawEllipse(position, width, height);

        ofSetColor(fillColor);
        ofDrawEllipse(position, std::max(0.0f, width - edgeWidth * 2), std::max(0.0f, height - edgeWidth * 2));
    }

  private:
    glm::vec2 position;
    float width;
    float height;
    float edgeWidth;
    ofColor edgeColor;
    ofColor fillColor;
};
}  // namespace engine