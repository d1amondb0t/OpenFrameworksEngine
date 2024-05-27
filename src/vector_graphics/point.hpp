#pragma once

#include "drawable.h"
#include "ofMain.h"

namespace engine {
class Point : public Drawable {
  public:
    Point(glm::vec2 position, float radius, ofColor color)
        : Drawable(), position(position), radius(radius), color(color) {}

    void draw() const override {
        ofFill();
        ofSetColor(color);
        ofDrawCircle(position, radius);
    }

  private:
    glm::vec2 position;
    float radius;
    ofColor color;
};
}  // namespace engine