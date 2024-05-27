#include "material_catalog.h"

namespace engine {

MaterialCatalog::MaterialCatalog() {
    ofDisableArbTex();

    setup();
}

MaterialCatalog::~MaterialCatalog() {
    for (auto* material : materials) {
        delete material;
    }
    materials.clear();
}

Material* MaterialCatalog::getMaterial(int index) {
    if (index >= materials.size()) {
        return nullptr;
    }

    return materials.at(index);
}

Material* MaterialCatalog::getMaterial(std::string name) {
    for (auto* material : materials) {
        if (material->getName() == name) {
            return material;
        }
    }

    return nullptr;
}

std::vector<Material*> MaterialCatalog::getMaterials() const {
    return materials;
}

Material* MaterialCatalog::newMaterial(std::string name) {
    Material* material = new Material(name);
    material->setAmbientColor(ofFloatColor(0.0f, 0.0f, 0.0f));
    material->setDiffuseColor(ofFloatColor(1.0f, 1.0f, 1.0f));
    material->setSpecularColor(ofFloatColor(1.0f, 1.0f, 1.0f));
    material->setShininess(32.0f);

    materials.push_back(material);

    return material;
}

void MaterialCatalog::setup() {
    std::shared_ptr<ofShader> lambert = std::make_shared<ofShader>();
    lambert->load("shaders/lambert_330_vs.glsl", "shaders/lambert_330_fs.glsl");
    std::shared_ptr<ofShader> gouraud = std::make_shared<ofShader>();
    gouraud->load("shaders/gouraud_330_vs.glsl", "shaders/gouraud_330_fs.glsl");
    std::shared_ptr<ofShader> phong = std::make_shared<ofShader>();
    phong->load("shaders/phong_330_vs.glsl", "shaders/phong_330_fs.glsl");
    std::shared_ptr<ofShader> blinnPhong = std::make_shared<ofShader>();
    blinnPhong->load("shaders/blinn_phong_330_vs.glsl", "shaders/blinn_phong_330_fs.glsl");
    std::shared_ptr<ofShader> toon = std::make_shared<ofShader>();
    toon->load("shaders/toon_330_vs.glsl", "shaders/toon_330_fs.glsl");
    std::shared_ptr<ofShader> normalMapBlinnPhong = std::make_shared<ofShader>();
    normalMapBlinnPhong->load("shaders/nmap_blinn_phong_330_vs.glsl", "shaders/nmap_blinn_phong_330_fs.glsl");

    auto* lambertMaterial = newMaterial("Lambert");
    lambertMaterial->setCustomShader(lambert);

    auto* gouraudMaterial = newMaterial("Gouraud");
    gouraudMaterial->setCustomShader(gouraud);

    auto* phongMaterial = newMaterial("Phong");
    phongMaterial->setCustomShader(phong);

    auto* blinnPhongMaterial = newMaterial("Blinn-Phong");
    blinnPhongMaterial->setCustomShader(blinnPhong);

    auto* toonMaterial = newMaterial("Toon");
    toonMaterial->setIsToon(true);
    toonMaterial->setCustomShader(toon);

    createPbrMaterial();
    createRatMaterial(normalMapBlinnPhong);
    createCatStatueMaterial(normalMapBlinnPhong);
}

void MaterialCatalog::createRatMaterial(std::shared_ptr<ofShader> shader) {
    auto* material = newMaterial("Rat");
    material->setCustomShader(shader);
    material->setShininess(512);

    material->loadTexture(OF_MATERIAL_TEXTURE_DIFFUSE, "3DObjects/rat/textures/street_rat_diff_4k.jpg");
    material->loadTexture(OF_MATERIAL_TEXTURE_NORMAL, "3DObjects/rat/textures/street_rat_nor_gl_4k.jpg");
}

void MaterialCatalog::createCatStatueMaterial(std::shared_ptr<ofShader> shader) {
    auto* material = newMaterial("Cat statue");
    material->setCustomShader(shader);
    material->setShininess(512);

    material->loadTexture(OF_MATERIAL_TEXTURE_DIFFUSE, "3DObjects/cat-statue/textures/concrete_cat_statue_diff_4k.jpg");
    material->loadTexture(OF_MATERIAL_TEXTURE_NORMAL,
                          "3DObjects/cat-statue/textures/concrete_cat_statue_nor_gl_4k.jpg");
}

void MaterialCatalog::createPbrMaterial() {
    std::shared_ptr<ofShader> pbr = std::make_shared<ofShader>();
    pbr->load("shaders/pbr_330_vs.glsl", "shaders/pbr_330_fs.glsl");

    auto* pbrMaterial = newMaterial("PBR");
    pbrMaterial->setCustomShader(pbr);
    pbrMaterial->setPBR(true);

    pbrMaterial->loadTexture(OF_MATERIAL_TEXTURE_DIFFUSE, "texture/metal_plate_diff_2k.jpg");
    pbrMaterial->loadTexture(OF_MATERIAL_TEXTURE_METALLIC, "texture/metal_plate_metal_2k.jpg");
    pbrMaterial->loadTexture(OF_MATERIAL_TEXTURE_ROUGHNESS, "texture/metal_plate_rough_2k.jpg");
    pbrMaterial->loadTexture(OF_MATERIAL_TEXTURE_OCCLUSION, "texture/metal_plate_ao_2k.jpg");

    pbrMaterial->setCustomUniform4f("mat_ambient", glm::vec4(0.f));
    pbrMaterial->setCustomUniform4f("mat_specular", glm::vec4(1.f));
    pbrMaterial->setCustomUniform1f("mat_shininess", 0.5f);
    pbrMaterial->setCustomUniform1f("mat_occlusion", 1.0f);
    pbrMaterial->setCustomUniform3f("mat_fresnel_ior", glm::vec3(0.04f, 0.04f, 0.04f));

    // Set Lighting
    pbrMaterial->setCustomUniform1f("light_intensity", 1.0f);
    pbrMaterial->setCustomUniform3f("light_color", glm::vec3(1.f, 1.f, 1.f));

    // Set Tone Mapping
    pbrMaterial->setCustomUniform1f("tone_mapping_exposure", 1.0f);
    pbrMaterial->setCustomUniform1f("tone_mapping_gamma", 2.2f);
    pbrMaterial->setCustomUniform1f("tone_mapping_toggle", false);
}

}  // namespace engine