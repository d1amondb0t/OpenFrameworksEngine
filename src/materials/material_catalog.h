#pragma once

#include <vector>

#include "material.h"

namespace engine {

class MaterialCatalog {
  public:
    MaterialCatalog();
    ~MaterialCatalog();

    Material* getMaterial(int index);
    Material* getMaterial(std::string name);
    std::vector<Material*> getMaterials() const;
    Material* newMaterial(std::string name);

  private:
    void setup();
    void createRatMaterial(std::shared_ptr<ofShader> shader);
    void createCatStatueMaterial(std::shared_ptr<ofShader> shader);
    void createPbrMaterial();

    std::vector<Material*> materials;
};

}  // namespace engine