#pragma once

#include <list>

#include "scene_element.h"
#include "scene_group.h"

namespace engine {

class Scene {
  public:
    Scene();
    ~Scene();

    ofEvent<void> beforeSelectionChange;
    ofEvent<void> afterSelectionChanged;
    ofEvent<void> afterSelectionHierarchyChanged;

    const list<SceneElement*>& getElements();
    const list<SceneElement*>& getSelectedElements();

    void add(SceneElement* element);
    void setParent(SceneElement* child, SceneElement* parent);
    void clearParent(SceneElement* element);

    void beforeSelectionChangeFct();
    void select(SceneElement* element);
    void toggleSelected(SceneElement* element);
    void deleteSelectedElements();

    void draw();

  private:
    void clearSelection();

    SceneGroup* scene;
    list<SceneElement*> selectedElements;
};

}  // namespace engine