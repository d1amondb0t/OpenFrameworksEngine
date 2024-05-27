#pragma once

#include <list>
#include <memory>

#include "../materials/material.h"
#include "../plans_coordonnees/convert_cartesian.h"

namespace engine {

class SceneElement {
  public:
    enum SceneState { invisible, surfaces, wireframe };

    SceneElement(uint32_t id, std::string name, ofNode* node, SceneElement* parent = nullptr);
    virtual ~SceneElement();

    const ofNode& getNode() const;
    const uint32_t getId() const;

    const std::string getName() const;
    void setName(std::string name);

    const SceneState getSceneState();
    void setSceneState(SceneState sceneState);

    glm::vec3 getRelativePosition() const;
    glm::vec3 getAbsolutePosition() const;

    ofQuaternion getRelativeRotation() const;
    ofQuaternion getAbsoluteRotation() const;

    glm::vec3 getRelativeScale() const;
    glm::vec3 getAbsoluteScale() const;

    void setRelativePosition(float px, float py, float pz = 0);
    void setRelativePosition(const ofVec3f& position);

    void setAbsolutePosition(float px, float py, float pz = 0);
    void setAbsolutePosition(const ofVec3f& position);

    void move(float x, float y, float z = 0);
    void move(const ofVec3f& offset);

    void setOrientation(const ofVec3f& eulerAngles);

    void rotate(float degrees, float vx, float vy, float vz = 0);
    void rotate(float degrees, ofVec3f v);
    void rotate(const ofQuaternion& q);

    void setRelativeScale(float x, float y, float z = 1);
    void setRelativeScale(const ofVec3f& scale);

    void scale(float x, float y, float z = 1);
    void scale(const ofVec3f& scale);

    bool isAncestor(const SceneElement* individual) const;
    bool isDescendant(const SceneElement* individual) const;
    void setParent(SceneElement* parent);
    void detachFromParent();
    void addChildAtEnd(SceneElement* child);
    void addChildAt(SceneElement* child, int index = 0);
    void removeChild(SceneElement* child);

    const std::list<SceneElement*>& getChildren();

    void select();
    void unselect();
    bool isSelected();

    virtual ofParameterGroup getAttributes(bool isMultiSelect);
    virtual void listen(ofParameterGroup attributesToListen, bool isMultiSelect);

    void beforeSelectionChange();
    static void staticbeforeSelectionChange();

    void onNameChanged(string& name);
    void onVisibilityChanged(bool& visibility);
    void onPositionChanged(glm::vec3& position);
    void onOrientationChanged(glm::vec3& orientation);
    void onScaleChanged(glm::vec3& scale);

    void onPositionAdded(glm::vec3& position);
    void onOrientationAdded(glm::vec3& orientation);
    void onScaleAdded(glm::vec3& scale);

    void onCylindricalCoordinatesChanged(glm::vec3& positionCylindrical);
    void onSphericalCoordinatesChanged(glm::vec3& positionSpherical);

    virtual void draw() = 0;

    virtual void drawGizmo() const;

    virtual const Material& getMaterial() const;
    virtual void setMaterial(Material* material) {}

  protected:
    ofNode* node;
    static ofNode gizmo;

  private:
    void updateSpareNode();

    static bool attrNameAlter;

    ofNode spareNode;
    const uint32_t id;
    std::string name;
    ofParameter<glm::vec3> position;
    ofParameter<glm::vec2> coordinatesPolar;
    ofParameter<glm::vec3> coordinatesCylindrical;
    ofParameter<glm::vec3> coordinatesSpherical;
    bool selected = false;
    /// \brief Value determining the visibility of the element in the scene.
    SceneState sceneState;

    SceneElement* parent;
    std::list<SceneElement*> children;
};

}  // namespace engine