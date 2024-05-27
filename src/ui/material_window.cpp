#include "material_window.h"

namespace engine {

MaterialWindow::MaterialWindow(std::shared_ptr<MaterialCatalog> materialCatalog) : materialCatalog(materialCatalog) {
    attributes.setName("Attributes");
}

void MaterialWindow::prepare(ofxImGui::Settings& settings) {
    ofxImGui::BeginWindow("Materials", settings);

    if (ImGui::BeginListBox("Materials list")) {
        int index = 0;
        for (auto* material : materialCatalog->getMaterials()) {
            bool isSelected = selectedMaterial == material;
            const std::string name = material->getName();

            if (ImGui::Selectable(name.c_str(), isSelected)) {
                onSelectionChanged(material);
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }

            if (ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload(MATERIAL_PAYLOAD.c_str(), &material, sizeof(Material*));

                ImGui::Text("%s", name.c_str());

                ImGui::EndDragDropSource();
            }

            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(TEXTURE_PAYLOAD.c_str())) {
                    ofTexture receivedTexture = *static_cast<ofTexture*>(payload->Data);
                    material->setTexture(OF_MATERIAL_TEXTURE_DIFFUSE, receivedTexture);
                    material->setTexture(OF_MATERIAL_TEXTURE_AMBIENT, receivedTexture);
                }

                ImGui::EndDragDropTarget();
            }

            index++;
        }

        ImGui::EndListBox();
    }

    ofxImGui::AddGroup(attributes, settings);

    if (selectedMaterial != nullptr) {
        if (ImGui::Button("Unbind textures")) {
            selectedMaterial->resetTextures();
        }
    }

    ofxImGui::EndWindow(settings);
}

void MaterialWindow::onSelectionChanged(Material* selectedMaterial) {
    this->selectedMaterial = selectedMaterial;

    attributes.clear();
    if (selectedMaterial != nullptr) {
        attributes = selectedMaterial->getAttributes();
    }
}

}  // namespace engine
