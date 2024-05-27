#pragma once

#include "ofMain.h"
#include "drawable.h"

namespace engine {
class Triangle : public Drawable {
  public:
    Triangle(
        glm::vec2 vertex1, glm::vec2 vertex2, glm::vec2 vertex3, float edgeWidth, ofColor edgeColor, ofColor fillColor)
        : Drawable(),
          vertex1(vertex1),
          vertex2(vertex2),
          vertex3(vertex3),
          edgeWidth(edgeWidth),
          edgeColor(edgeColor),
          fillColor(fillColor) {}

    void draw() const override {
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(fillColor);
        ofDrawTriangle(vertex1, vertex2, vertex3);

        ofNoFill();
        ofSetLineWidth(edgeWidth);
        ofSetColor(edgeColor);
        ofDrawTriangle(vertex1, vertex2, vertex3);
    }

  private:
    glm::vec2 vertex1;
    glm::vec2 vertex2;
    glm::vec2 vertex3;
    float edgeWidth;
    ofColor edgeColor;
    ofColor fillColor;
};
}  // namespace engine