#pragma once

#include <memory>

#include "../materials/material_catalog.h"
#include "../scene/scene.h"
#include "../scene/scene_element_factory.h"
#include "ImHelpers.h"
#include "material_payload.hpp"
#include "ofxImGui.h"

namespace engine {

class SceneGraphWindow {
    static const std::string CONTEXTUAL_MENU_NAME;
    static const std::string SCENE_ELEMENT_PAYLOAD;

  public:
    SceneGraphWindow(std::shared_ptr<Scene> scene, std::shared_ptr<MaterialCatalog> materialCatalog);

    void prepare(ofxImGui::Settings& settings);

  private:
    void prepareContextualMenu();

    void buildSceneTree();
    void buildSceneElementBranch(SceneElement* element, ImGuiTreeNodeFlags baseFlags);
    void addDragDrop(SceneElement* element);

    std::shared_ptr<Scene> scene;
    SceneElementFactory sceneElementFactory;

    SceneElement* clickedElement;
};

}  // namespace engine
