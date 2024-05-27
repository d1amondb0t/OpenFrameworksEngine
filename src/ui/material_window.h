#pragma once

#include <memory>

#include "../materials/material_catalog.h"
#include "../scene/scene.h"
#include "../scene/scene_element_factory.h"
#include "ImHelpers.h"
#include "material_payload.hpp"
#include "ofxImGui.h"
#include "texture_payload.hpp"

namespace engine {

class MaterialWindow {
  public:
    MaterialWindow(std::shared_ptr<MaterialCatalog> materialCatalog);

    void prepare(ofxImGui::Settings& settings);

  private:
    void onSelectionChanged(Material* selectedMaterial);

    std::shared_ptr<MaterialCatalog> materialCatalog;

    Material* selectedMaterial;
    ofParameterGroup attributes;
};

}  // namespace engine
