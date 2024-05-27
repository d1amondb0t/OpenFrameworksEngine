#include "element_attribute_window.h"

namespace engine {

ElementAttributeWindow::ElementAttributeWindow(std::shared_ptr<Scene> scene) : scene(scene) {
    ofAddListener(scene->beforeSelectionChange, this, &ElementAttributeWindow::beforeSelectionChange);
    ofAddListener(scene->afterSelectionChanged, this, &ElementAttributeWindow::afterSelectionChanged);
    ofAddListener(scene->afterSelectionHierarchyChanged, this, &ElementAttributeWindow::afterSelectionHierarchyChanged);
}

ElementAttributeWindow::~ElementAttributeWindow() {
    ofRemoveListener(scene->beforeSelectionChange, this, &ElementAttributeWindow::beforeSelectionChange);
    ofRemoveListener(scene->afterSelectionChanged, this, &ElementAttributeWindow::afterSelectionChanged);
    ofRemoveListener(
        scene->afterSelectionHierarchyChanged, this, &ElementAttributeWindow::afterSelectionHierarchyChanged);
}

void ElementAttributeWindow::prepare(ofxImGui::Settings& settings) {
    ofxImGui::BeginWindow("Properties", settings);

    if (attributes.size() > 0) {
        ofxImGui::AddGroup(attributes, settings);
    }

    ofxImGui::EndWindow(settings);
}

void ElementAttributeWindow::beforeSelectionChange() {
    attributes.clear();
}

void ElementAttributeWindow::afterSelectionChanged() {
    loadAttributesValues();
}

void ElementAttributeWindow::afterSelectionHierarchyChanged() {
    loadAttributesValues();
}

void ElementAttributeWindow::afterReturnKeyReleased() {
    loadAttributesValues();
}

void ElementAttributeWindow::loadAttributesValues() {
    const std::list<SceneElement*> selection = scene->getSelectedElements();

    size_t selected = selection.size();
    if (selected == 0) {
        attributes.clear();
    } else {
        attributes = selection.front()->getAttributes(selected > 1);
        bool isCommonAttr;
        // The attributes of the first element are already in the variable, so we skip to the second
        for (auto elem_itr = ++selection.begin(); elem_itr != selection.end(); elem_itr++) {
            for (auto commonAttr : attributes) {
                isCommonAttr = false;
                for (auto elemAttr : (*elem_itr)->getAttributes(true)) {
                    if (commonAttr->getName() == elemAttr->getName()) {
                        isCommonAttr = true;
                        break;
                    }
                }
                if (!isCommonAttr) {
                    attributes.remove(*commonAttr);
                }
            }
        }
        for (auto elem_itr = selection.begin(); elem_itr != selection.end(); elem_itr++) {
            (*elem_itr)->listen(attributes, selected > 1);
        }
    }
}

}  // namespace engine
