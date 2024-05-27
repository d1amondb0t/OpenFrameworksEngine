#pragma once

#include <memory>

#include "ImHelpers.h"
#include "ofxImGui.h"
#include "../scene/scene.h"

namespace engine {

class ElementAttributeWindow {
  public:
    ElementAttributeWindow(std::shared_ptr<Scene> scene);
    ~ElementAttributeWindow();

    void prepare(ofxImGui::Settings& settings);
    void beforeSelectionChange();
    void afterSelectionChanged();
    void afterSelectionHierarchyChanged();
    void afterReturnKeyReleased();

  private:
    void loadAttributesValues();

    std::shared_ptr<Scene> scene;

    ofParameterGroup attributes;
};

}  // namespace engine
