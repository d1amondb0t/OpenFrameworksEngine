#include "scene_element.h"

namespace engine {

bool SceneElement::attrNameAlter = false;

ofNode SceneElement::gizmo = ofNode();

void SceneElement::staticbeforeSelectionChange() {
    // Trying not to spread attribute values to new selection
    attrNameAlter = !attrNameAlter;
}

SceneElement::SceneElement(uint32_t id, std::string name, ofNode* node, SceneElement* parent)
    : node(node), id(id), name(name), sceneState(SceneState::surfaces), parent(parent) {
    if (parent != nullptr) {
        node->setParent(*parent->node);
    }

    position.set("Position", getAbsolutePosition(), glm::vec3(-5000), glm::vec3(5000));
    position.addListener(this, &SceneElement::onPositionChanged);

    coordinatesPolar.set("Polar Coordinates (Read-only)",
                         ConvertCartesian::convertCartesianToPolar(getAbsolutePosition()),
                         glm::vec2(-5000, -3.6),
                         glm::vec2(5000, 3.6));

    coordinatesCylindrical.set("Cylindrical Coordinates",
                               ConvertCartesian::convertCartesianToCylindrical(getAbsolutePosition()),
                               glm::vec3(-5000, -3.6f, -5000),
                               glm::vec3(5000, 3.6f, 5000));

    coordinatesSpherical.set("Spherical Coordinates",
                             ConvertCartesian::convertCartesianToSpherical(getAbsolutePosition()),
                             glm::vec3(-5000, -3.6f, -3.6f),
                             glm::vec3(5000, 3.6f, 3.6f));

    coordinatesCylindrical.addListener(this, &SceneElement::onCylindricalCoordinatesChanged);
    coordinatesSpherical.addListener(this, &SceneElement::onSphericalCoordinatesChanged);
}
SceneElement::~SceneElement() {
    for (std::list<SceneElement*>::const_iterator child_itr = children.cbegin(); child_itr != children.cend();
         child_itr++) {
        delete *child_itr;
    }
    delete node;
}

const ofNode& SceneElement::getNode() const {
    return *this->node;
}

const uint32_t SceneElement::getId() const {
    return id;
}

const std::string SceneElement::getName() const {
    return name;
}

void SceneElement::setName(std::string name) {
    this->name = name;
}

const SceneElement::SceneState SceneElement::getSceneState() {
    return sceneState;
}

void SceneElement::setSceneState(SceneElement::SceneState sceneState) {
    this->sceneState = sceneState;
}

glm::vec3 SceneElement::getRelativePosition() const {
    return node->getPosition();
}
glm::vec3 SceneElement::getAbsolutePosition() const {
    return node->getGlobalPosition();
}

ofQuaternion SceneElement::getRelativeRotation() const {
    return node->getOrientationQuat();
}
ofQuaternion SceneElement::getAbsoluteRotation() const {
    return node->getGlobalOrientation();
}

glm::vec3 SceneElement::getRelativeScale() const {
    return node->getScale();
}
glm::vec3 SceneElement::getAbsoluteScale() const {
    return node->getGlobalScale();
}

void SceneElement::setRelativePosition(float px, float py, float pz) {
    node->setPosition(px, py, pz);
}
void SceneElement::setRelativePosition(const ofVec3f& position) {
    node->setPosition(position);
}

void SceneElement::setAbsolutePosition(float px, float py, float pz) {
    setAbsolutePosition(ofVec3f(px, py, pz));
}

void SceneElement::setAbsolutePosition(const ofVec3f& position) {
    setRelativePosition((parent != nullptr ? position - parent->getAbsolutePosition() : position));
}

void SceneElement::move(float x, float y, float z) {
    node->move(x, y, z);
}
void SceneElement::move(const ofVec3f& offset) {
    node->move(offset);
}

void SceneElement::setOrientation(const ofVec3f& eulerAngles) {
    node->setOrientation(eulerAngles);
}

void SceneElement::rotate(float degrees, float vx, float vy, float vz) {
    node->rotateDeg(degrees, vx, vy, vz);
}
void SceneElement::rotate(float degrees, ofVec3f v) {
    node->rotateDeg(degrees, v);
}
void SceneElement::rotate(const ofQuaternion& q) {
    node->rotate(q);
}

void SceneElement::setRelativeScale(float x, float y, float z) {
    node->setScale(x, y, z);
}
void SceneElement::setRelativeScale(const ofVec3f& scale) {
    node->setScale(scale);
}

void SceneElement::scale(float x, float y, float z) {
    ofVec3f currentScale = node->getScale();
    node->setScale(currentScale.x * x, currentScale.y * y, currentScale.z * z);
}
void SceneElement::scale(const ofVec3f& scale) {
    ofVec3f currentScale = node->getScale();
    node->setScale(currentScale.x * scale.x, currentScale.y * scale.y, currentScale.z * scale.z);
}

bool SceneElement::isAncestor(const SceneElement* individual) const {
    return individual == this || (parent != nullptr && parent->isAncestor(individual));
}
bool SceneElement::isDescendant(const SceneElement* individual) const {
    bool isDescendant = individual == this;
    for (std::list<SceneElement*>::const_iterator child_itr = children.cbegin();
         !isDescendant && child_itr != children.cend();
         child_itr++) {
        isDescendant |= (*child_itr)->isDescendant(individual);
    }
    return isDescendant;
}
void SceneElement::setParent(SceneElement* parent) {
    parent->addChildAtEnd(this);
}
void SceneElement::detachFromParent() {
    parent->removeChild(this);
}

void SceneElement::addChildAtEnd(SceneElement* child) {
    addChildAt(child, children.size());
}
void SceneElement::addChildAt(SceneElement* child, int index) {
    if (isAncestor(child)) {
        throw(std::logic_error("Element can't be its own ancestor."));
    }

    if (std::find(children.begin(), children.end(), child) != children.end()) {
        return;
    }

    if (child->parent != nullptr) {
        child->parent->removeChild(child);
    }

    child->node->setParent(*this->node);

    std::list<SceneElement*>::const_iterator next = children.begin();
    for (int i = 0; i < index; ++i) {
        ++next;
    }
    children.insert(next, child);
    child->parent = this;
}

void SceneElement::removeChild(SceneElement* child) {
    child->parent = nullptr;
    child->node->clearParent(true);
    children.remove(child);
}

const std::list<SceneElement*>& SceneElement::getChildren() {
    return children;
}

void SceneElement::updateSpareNode() {
    spareNode.setScale(node->getScale());
    spareNode.setOrientation(node->getOrientationEuler());
    spareNode.setPosition(node->getPosition());
}

void SceneElement::beforeSelectionChange() {
    updateSpareNode();
}

void SceneElement::select() {
    selected = true;
    updateSpareNode();
}

void SceneElement::unselect() {
    selected = false;
}

bool SceneElement::isSelected() {
    return selected;
}

ofParameterGroup SceneElement::getAttributes(bool isMultiSelect) {
    string AlterSuffix = string(attrNameAlter ? " " : "");

    ofParameterGroup parameters = ofParameterGroup("Attributes" + AlterSuffix);

    ofParameter<string> name;
    if (!isMultiSelect) {
        name.set("Name", this->name);
    } else {
        name.set("Selection");
    }

    ofParameter<bool> visibility;
    visibility.set("Visibility", isMultiSelect ? false : this->sceneState == surfaces);

    ofParameter<glm::vec3> position;
    position.set("Position", isMultiSelect ? glm::vec3() : getRelativePosition(), glm::vec3(-5000), glm::vec3(5000));

    ofParameter<glm::vec3> orientation;
    orientation.set("Orientation", isMultiSelect ? glm::vec3() : node->getOrientationEulerDeg(), glm::vec3(-360), glm::vec3(360));

    ofParameter<glm::vec3> scale;
    scale.set("Scale", isMultiSelect ? glm::vec3(1.0f) : getAbsoluteScale(), glm::vec3(0.01f), glm::vec3(100.0f));

    parameters.add(name);
    parameters.add(visibility);
    parameters.add(position);
    parameters.add(orientation);
    parameters.add(scale);
    parameters.add(coordinatesPolar);
    parameters.add(coordinatesCylindrical);
    parameters.add(coordinatesSpherical);

    return parameters;
}

void SceneElement::listen(ofParameterGroup attributesToListen, bool isMultiSelect) {
    string attrName;
    for (auto attr : attributesToListen) {
        attrName = attr->getName();

        if (attrName == "Visibility") {
            attr->cast<bool>().addListener(this, &SceneElement::onVisibilityChanged);
        } else if (isMultiSelect) {
            if (attrName == "Position") {
                attr->cast<glm::vec3>().addListener(this, &SceneElement::onPositionAdded);
            } else if (attrName == "Orientation") {
                attr->cast<glm::vec3>().addListener(this, &SceneElement::onOrientationAdded);
            } else if (attrName == "Scale") {
                attr->cast<glm::vec3>().addListener(this, &SceneElement::onScaleAdded);
            }
        } else {
            if (attrName == "Name") {
                attr->cast<string>().addListener(this, &SceneElement::onNameChanged);
            } else if (attrName == "Position") {
                attr->cast<glm::vec3>().addListener(this, &SceneElement::onPositionChanged);
            } else if (attrName == "Orientation") {
                attr->cast<glm::vec3>().addListener(this, &SceneElement::onOrientationChanged);
            } else if (attrName == "Scale") {
                attr->cast<glm::vec3>().addListener(this, &SceneElement::onScaleChanged);
            }
        }
    }
}

void SceneElement::onNameChanged(string& name) {
    setName(name);
}

void SceneElement::onVisibilityChanged(bool& visibility) {
    setSceneState(visibility ? SceneState::surfaces : SceneState::invisible);
}

void SceneElement::onPositionChanged(glm::vec3& positionT) {
    coordinatesCylindrical.disableEvents();
    coordinatesSpherical.disableEvents();
    setAbsolutePosition(positionT);

    coordinatesPolar = ConvertCartesian::convertCartesianToPolar(positionT);
    coordinatesCylindrical = ConvertCartesian::convertCartesianToCylindrical(positionT);
    coordinatesSpherical = ConvertCartesian::convertCartesianToSpherical(positionT);

    coordinatesCylindrical.enableEvents();
    coordinatesSpherical.enableEvents();
}

void SceneElement::onOrientationChanged(glm::vec3& orientation) {
    setOrientation(orientation);
}

void SceneElement::onScaleChanged(glm::vec3& scale) {
    setRelativeScale(scale);
}

void SceneElement::onPositionAdded(glm::vec3& position) {
    node->setPosition(spareNode.getPosition() + position);
}

void SceneElement::onOrientationAdded(glm::vec3& orientation) {
    node->setOrientation(spareNode.getOrientationEulerDeg() + orientation);
}

void SceneElement::onScaleAdded(glm::vec3& scale) {
    node->setScale(spareNode.getScale() * scale);
}

void SceneElement::onCylindricalCoordinatesChanged(glm::vec3& positionCylindrical) {
    position.disableEvents();
    coordinatesSpherical.disableEvents();

    auto positionT = ConvertCartesian::convertCylindricalToCartesian(positionCylindrical);
    setAbsolutePosition(positionT);

    position = positionT;
    coordinatesPolar = ConvertCartesian::convertCartesianToPolar(positionT);
    coordinatesSpherical = ConvertCartesian::convertCartesianToSpherical(positionT);

    position.enableEvents();
    coordinatesSpherical.enableEvents();
}

void SceneElement::onSphericalCoordinatesChanged(glm::vec3& positionSpherical) {
    position.disableEvents();
    coordinatesCylindrical.disableEvents();

    auto positionT = ConvertCartesian::convertCylindricalToCartesian(positionSpherical);
    setAbsolutePosition(positionT);

    position = positionT;
    coordinatesPolar = ConvertCartesian::convertCartesianToPolar(positionT);
    coordinatesSpherical = ConvertCartesian::convertCartesianToSpherical(positionT);

    position.enableEvents();
    coordinatesCylindrical.enableEvents();
}

void SceneElement::drawGizmo() const {
    float angle, x, y, z;
    ofPushMatrix();
    ofTranslate(this->getAbsolutePosition());
    this->getAbsoluteRotation().getRotate(angle, x, y, z);
    ofRotate(angle, x, y, z);
    ofScale(1.0f, 1.0f, 1.0f);
    gizmo.draw();
    ofPopMatrix();
}

const Material& SceneElement::getMaterial() const {
    return Material();
};

}  // namespace engine