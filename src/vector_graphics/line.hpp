#pragma once

#include "ofMain.h"
#include "drawable.h"

namespace engine {
class Line : public Drawable {
  public:
    Line(glm::vec2 startPosition, glm::vec2 endPosition, float width, ofColor color)
        : Drawable(), startPosition(startPosition), endPosition(endPosition), width(width), color(color) {}

    void draw() const override {
        ofNoFill();
        ofSetColor(color);
        ofSetLineWidth(width);
        ofDrawLine(startPosition, endPosition);
    }

  private:
    glm::vec2 startPosition;
    glm::vec2 endPosition;
    float width;
    ofColor color;
};
}  // namespace engine