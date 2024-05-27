#pragma once

#include "drawable.h"
#include "ofMain.h"

namespace engine {
class Rectangle : public Drawable {
  public:
    Rectangle(glm::vec2 topLeftCorner, float width, float height, float edgeWidth, ofColor edgeColor, ofColor fillColor)
        : Drawable(),
          topLeftCorner(topLeftCorner),
          width(width),
          height(height),
          edgeWidth(edgeWidth),
          edgeColor(edgeColor),
          fillColor(fillColor) {}

    void draw() const override {
        ofFill();

        ofSetColor(edgeColor);
        ofDrawRectangle(topLeftCorner.x, topLeftCorner.y, width, height);

        ofSetColor(fillColor);
        ofDrawRectangle(topLeftCorner.x + edgeWidth,
                        topLeftCorner.y + edgeWidth,
                        std::max(0.0f, width - (2 * edgeWidth)),
                        std::max(0.0f, height - (2 * edgeWidth)));
    }

  private:
    glm::vec2 topLeftCorner;
    float width;
    float height;
    float edgeWidth;
    ofColor edgeColor;
    ofColor fillColor;
};
}  // namespace engine