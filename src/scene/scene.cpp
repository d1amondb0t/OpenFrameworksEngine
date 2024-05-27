#include "scene.h"

namespace engine {

Scene::Scene() {
    scene = new SceneGroup(0, "Root");
    selectedElements = std::list<SceneElement*>();
}

Scene::~Scene() {
    delete scene;

    selectedElements.clear();
}

const std::list<SceneElement*>& Scene::getElements() {
    return scene->getChildren();
}

const std::list<SceneElement*>& Scene::getSelectedElements() {
    return selectedElements;
}

void Scene::add(SceneElement* element) {
    scene->addChildAtEnd(element);
}

void Scene::setParent(SceneElement* child, SceneElement* parent) {
    parent->addChildAtEnd(child);

    if (child->isSelected()) {
        ofNotifyEvent(afterSelectionHierarchyChanged);
    }
}

void Scene::clearParent(SceneElement* element) {
    setParent(element, scene);
}

void Scene::beforeSelectionChangeFct() {
    for (auto* element : selectedElements) {
        element->beforeSelectionChange();
    }
    SceneElement::staticbeforeSelectionChange();
}

void Scene::select(SceneElement* element) {
    clearSelection();

    toggleSelected(element);
}

void Scene::toggleSelected(SceneElement* element) {
    beforeSelectionChangeFct();
    ofNotifyEvent(beforeSelectionChange);
    if (element->isSelected()) {
        element->unselect();
        selectedElements.remove(element);
    } else {
        element->select();
        selectedElements.push_back(element);
    }

    ofNotifyEvent(afterSelectionChanged);
}

void Scene::deleteSelectedElements() {
    beforeSelectionChangeFct();
    ofNotifyEvent(beforeSelectionChange);
    for (auto* element : selectedElements) {
        element->detachFromParent();

        delete element;
    }

    selectedElements.clear();
    ofNotifyEvent(afterSelectionChanged);
}

void addChildren(SceneElement* element, std::list<SceneElement*>& elements) {
    elements.push_back(element);

    for (auto* child : element->getChildren()) {
        addChildren(child, elements);
    }
}

void Scene::draw() {
    std::list<SceneElement*> allElements;
    for (auto* element : scene->getChildren()) {
        addChildren(element, allElements);
    }

    for (auto* element : allElements) {
        switch (element->getSceneState()) {
            case SceneElement::SceneState::invisible:
                continue;
            case SceneElement::SceneState::surfaces:
                break;
            case SceneElement::SceneState::wireframe:
                break;
        }
        element->draw();
    }
}

void Scene::clearSelection() {
    for (auto* element : selectedElements) {
        element->unselect();
    }

    selectedElements.clear();
}

}  // namespace engine
