#include "scene_graph_window.h"

namespace engine {
const std::string SceneGraphWindow::CONTEXTUAL_MENU_NAME = "SceneGraphContextualMenu";
const std::string SceneGraphWindow::SCENE_ELEMENT_PAYLOAD = "SceneElementPayload";

SceneGraphWindow::SceneGraphWindow(std::shared_ptr<Scene> scene, std::shared_ptr<MaterialCatalog> materialCatalog)
    : scene(scene), sceneElementFactory(materialCatalog) {
    clickedElement = nullptr;
}

void SceneGraphWindow::prepare(ofxImGui::Settings& settings) {
    ofxImGui::BeginWindow("Scene", settings);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
        ImGui::OpenPopup(CONTEXTUAL_MENU_NAME.c_str());
    }

    prepareContextualMenu();

    buildSceneTree();

    ImGui::Dummy(ImGui::GetContentRegionAvail());

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(SCENE_ELEMENT_PAYLOAD.c_str())) {
            SceneElement* receivedElement = *static_cast<SceneElement**>(payload->Data);
            if (receivedElement != nullptr) {
                scene->clearParent(receivedElement);
            }
        }

        ImGui::EndDragDropTarget();
    }

    ofxImGui::EndWindow(settings);
}

void SceneGraphWindow::prepareContextualMenu() {
    if (ImGui::BeginPopupContextWindow(CONTEXTUAL_MENU_NAME.c_str())) {
        if (ImGui::BeginMenu("New")) {
            if (ImGui::MenuItem("Group")) {
                auto* group = sceneElementFactory.createSceneGroup();
                scene->add(group);
            }

            if (ImGui::MenuItem("Light")) {
                auto* light = sceneElementFactory.createSceneLight();
                scene->add(light);
            }

            if (ImGui::BeginMenu("3D Element")) {
                if (ImGui::MenuItem("Curve")) {
                    auto* curve = sceneElementFactory.createSceneParametricCurve();
                    scene->add(curve);
                }
                if (ImGui::MenuItem("Plane")) {
                    auto* plane = sceneElementFactory.createScenePlane();
                    scene->add(plane);
                }
                if (ImGui::MenuItem("Box")) {
                    auto* box = sceneElementFactory.createSceneBox();
                    scene->add(box);
                }
                if (ImGui::MenuItem("Sphere")) {
                    auto* sphere = sceneElementFactory.createSceneSphere();
                    scene->add(sphere);
                }
                if (ImGui::MenuItem("Cone")) {
                    auto* cone = sceneElementFactory.createSceneCone();
                    scene->add(cone);
                }
                if (ImGui::MenuItem("Teapot")) {
                    auto* teapot = sceneElementFactory.createScene3dModel("3DObjects/teapot.obj");
                    scene->add(teapot);
                }
                if (ImGui::MenuItem("Room")) {
                    auto* room = sceneElementFactory.createScene3dModel("3DObjects/room.fbx");
                    scene->add(room);
                }
                if (ImGui::MenuItem("Dragon")) {
                    auto* dragon = sceneElementFactory.createScene3dModel("3DObjects/dragon.fbx");
                    scene->add(dragon);
                }
                if (ImGui::MenuItem("Rat")) {
                    auto* rat = sceneElementFactory.createScene3dModel("3DObjects/rat/street_rat_4k.gltf", "Rat");
                    scene->add(rat);
                }
                if (ImGui::MenuItem("Cat statue")) {
                    auto* catStatue = sceneElementFactory.createScene3dModel(
                        "3DObjects/cat-statue/concrete_cat_statue_4k.gltf", "Cat statue");
                    scene->add(catStatue);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (!scene->getSelectedElements().empty() && ImGui::MenuItem("Delete")) {
            scene->deleteSelectedElements();
        }

        ImGui::EndPopup();
    }
}

void SceneGraphWindow::buildSceneTree() {
    clickedElement = nullptr;

    ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                   ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

    for (auto* element : scene->getElements()) {
        buildSceneElementBranch(element, baseFlags);
    }

    if (clickedElement != nullptr) {
        if (ImGui::GetIO().KeyCtrl) {
            scene->toggleSelected(clickedElement);
        } else {
            scene->select(clickedElement);
        }
    }
    clickedElement = nullptr;
}

void SceneGraphWindow::buildSceneElementBranch(SceneElement* element, ImGuiTreeNodeFlags baseFlags) {
    ImGuiTreeNodeFlags nodeFlags = baseFlags;
    if (element->isSelected()) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    auto children = element->getChildren();
    if (children.empty()) {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        ImGui::TreeNodeEx((void*)(uintptr_t)element->getId(), nodeFlags, "%s", element->getName().c_str());

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            clickedElement = element;
        }

        addDragDrop(element);

    } else {
        bool isNodeOpen =
            ImGui::TreeNodeEx((void*)(uintptr_t)element->getId(), nodeFlags, "%s", element->getName().c_str());

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            clickedElement = element;
        }

        addDragDrop(element);

        if (isNodeOpen) {
            for (auto* child : children) {
                buildSceneElementBranch(child, baseFlags);
            }

            ImGui::TreePop();
        }
    }
}

void SceneGraphWindow::addDragDrop(SceneElement* element) {
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(SCENE_ELEMENT_PAYLOAD.c_str(), &element, sizeof(SceneElement*));

        ImGui::Text("%s", element->getName().c_str());

        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::GetDragDropPayload()) {
            SceneElement* receivedElement = *static_cast<SceneElement**>(payload->Data);
            if (element->isAncestor(receivedElement)) {
                ImGui::TextColored(ImVec4(ofColor::red), "An element cannot be its own ancestor");
            }
        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(SCENE_ELEMENT_PAYLOAD.c_str())) {
            SceneElement* receivedElement = *static_cast<SceneElement**>(payload->Data);
            if (receivedElement != nullptr) {
                if (!element->isAncestor(receivedElement)) {
                    scene->setParent(receivedElement, element);
                }
            }
        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MATERIAL_PAYLOAD.c_str())) {
            Material* receivedMaterial = *static_cast<Material**>(payload->Data);
            element->setMaterial(receivedMaterial);
        }

        ImGui::EndDragDropTarget();
    }
}

}  // namespace engine
