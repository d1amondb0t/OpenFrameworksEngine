#include "scene_parametric_curve.h"

namespace engine {

SceneParametricCurve::SceneParametricCurve(uint32_t id, std::string name, Material* material, SceneElement* parent, int segmentCount)
    : Scene3DElement(id, name, new ofNode(), material, parent), curvePoints(), p1(nullptr), p2(nullptr), p3(nullptr), p4(nullptr), subdivisionCount(30), segmentCount(segmentCount), currentModifiedSegment(1), curve(), t1(), t2() {
    if (segmentCount == 1) {
        curvePoints.push_back(glm::vec3(-90.0f, 0.0f, 0.0f));
        curvePoints.push_back(glm::vec3(-70.0f, 100.0f, 0.0f));
        curvePoints.push_back(glm::vec3(70.0f, 100.0f, 0.0f));
        curvePoints.push_back(glm::vec3(90.0f, 0.0f, 0.0f));
        p1 = &curvePoints.at(0);
        p2 = &curvePoints.at(1);
        p3 = &curvePoints.at(2);
        p4 = &curvePoints.at(3);
        build();
    }
}

SceneParametricCurve::~SceneParametricCurve() {}

glm::vec3 SceneParametricCurve::buildVertex(double t, int startIndex) {
    glm::vec3 p1 = curvePoints.at(startIndex);
    glm::vec3 p2 = curvePoints.at(startIndex + 1);
    glm::vec3 p3 = curvePoints.at(startIndex + 2);
    glm::vec3 p4 = curvePoints.at(startIndex + 3);

    return (pow(1 - t, 3) * p1) + (3 * t * pow(1 - t, 2) * p2) + (3 * pow(t, 2) * (1 - t) * p3) + (pow(t, 3) * p4);
}

void SceneParametricCurve::buildSegment(int startIndex) {

    double fraction = 1.0 / subdivisionCount;
    double nextSegment = 0.0;
    glm::vec3 vertex;
    int index;

    // Bézier cubique
    for (int i = 0; i <= subdivisionCount; i++, nextSegment += fraction) {
        vertex = buildVertex(nextSegment, startIndex);
        index = i + ((startIndex * subdivisionCount) / 3);
        if (index == curve.size()) {
            curve.addVertex(vertex);
        }
    }
}

void SceneParametricCurve::build() {
    curve.clear();

    for (int i = 0; i <= (segmentCount * 3) - 3; i += 3) {
        buildSegment(i);
    }

    p1 = &curvePoints.at((currentModifiedSegment * 3) - 3);
    p2 = &curvePoints.at((currentModifiedSegment * 3) - 2);
    p3 = &curvePoints.at((currentModifiedSegment * 3) - 1);
    p4 = &curvePoints.at((currentModifiedSegment * 3));

    t1.clear();
    t2.clear();
    t1.addVertex(*p1);
    t1.addVertex(*p2);
    t2.addVertex(*p4);
    t2.addVertex(*p3);
}

glm::vec3 SceneParametricCurve::getPoint(int index) const {
    switch (index) {
        case 1:
            return *p1;
        case 2:
            return *p2;
        case 3:
            return *p3;
        case 4:
            return *p4;
        default:
            return getPoint((index % 4) + 1);
    }
}

void SceneParametricCurve::setPoint(int index, glm::vec3 point) {
    switch (index) {
        case 1:
            *p1 = point;
            break;
        case 2:
            *p2 = point;
            break;
        case 3:
            *p3 = point;
            break;
        case 4:
            *p4 = point;
            break;
        default:
            setPoint((index % 4) + 1, point);
            break;
    }
}

ofParameterGroup SceneParametricCurve::getAttributes(bool isMultiSelect) {
    ofParameterGroup parameters = SceneElement::getAttributes(isMultiSelect);

    ofParameter<int> subdivisionCount;
    subdivisionCount.set("Subdivision Count", this->subdivisionCount, 1, 1000);

    ofParameter<int> segmentCount;
    segmentCount.set("Segment Count", this->segmentCount, 1, 10);

    ofParameter<int> currentModifiedSegment;
    currentModifiedSegment.set("Current Segment", this->currentModifiedSegment, 1, segmentCount);

    ofParameter<glm::vec3> p1;
    p1.set("Point 1", *this->p1, glm::vec3(-5000), glm::vec3(5000));

    ofParameter<glm::vec3> p2;
    p2.set("Point 2", *this->p2, glm::vec3(-5000), glm::vec3(5000));

    ofParameter<glm::vec3> p3;
    p3.set("Point 3", *this->p3, glm::vec3(-5000), glm::vec3(5000));

    ofParameter<glm::vec3> p4;
    p4.set("Point 4", *this->p4, glm::vec3(-5000), glm::vec3(5000));

    parameters.add(subdivisionCount);
    parameters.add(segmentCount);
    parameters.add(currentModifiedSegment);
    parameters.add(p1);
    parameters.add(p2);
    parameters.add(p3);
    parameters.add(p4);

    return parameters;
}

void SceneParametricCurve::listen(ofParameterGroup attributesToListen, bool isMultiSelect) {
    SceneElement::listen(attributesToListen, isMultiSelect);

    string attrName;
    for (auto attr : attributesToListen) {
        attrName = attr->getName();

        if (attrName == "Subdivision Count") {
            attr->cast<int>().addListener(this, &SceneParametricCurve::onSubdivisionCountChanged);
        } else if (attrName == "Segment Count") {
            attr->cast<int>().addListener(this, &SceneParametricCurve::onSegmentCountChanged);
        } else if (attrName == "Current Segment") {
            attr->cast<int>().addListener(this, &SceneParametricCurve::onCurrentSegmentChanged);
        } else if (attrName == "Point 1") {
            attr->cast<glm::vec3>().addListener(this, &SceneParametricCurve::onPoint1Changed);
        } else if (attrName == "Point 2") {
            attr->cast<glm::vec3>().addListener(this, &SceneParametricCurve::onPoint2Changed);
        } else if (attrName == "Point 3") {
            attr->cast<glm::vec3>().addListener(this, &SceneParametricCurve::onPoint3Changed);
        } else if (attrName == "Point 4") {
            attr->cast<glm::vec3>().addListener(this, &SceneParametricCurve::onPoint4Changed);
        }
    }
}

void SceneParametricCurve::onSubdivisionCountChanged(int& subdivisionCount) {
    this->subdivisionCount = subdivisionCount;
    build();
}

void SceneParametricCurve::onSegmentCountChanged(int& segmentCount) {
    int difference = segmentCount - this->segmentCount;
    if (difference > 0) {
        glm::vec3 lastPointOffset;
        int lastPointIndex = this->segmentCount * 3;
        for (int i = 0; i < difference; i++) {
            lastPointOffset = curvePoints.back() - curvePoints.at(lastPointIndex - 3);
            curvePoints.push_back((lastPointOffset + curvePoints.at(lastPointIndex - 2)));
            curvePoints.push_back((lastPointOffset + curvePoints.at(lastPointIndex - 1)));
            curvePoints.push_back((lastPointOffset + curvePoints.at(lastPointIndex)));
        }
    } else {
        for (int i = 0; i > difference; i--) {
            curvePoints.pop_back();
            curvePoints.pop_back();
            curvePoints.pop_back();
        }
    }

    this->segmentCount = segmentCount;
    if (currentModifiedSegment > segmentCount) {
        currentModifiedSegment = segmentCount;
    }
    build();
}

void SceneParametricCurve::onCurrentSegmentChanged(int& currentModifiedSegment) {
    this->currentModifiedSegment = currentModifiedSegment;
    int i = 3 * (currentModifiedSegment - 1);
    p1 = &curvePoints.at(i);
    p2 = &curvePoints.at(i + 1);
    p3 = &curvePoints.at(i + 2);
    p4 = &curvePoints.at(i + 3);
    build();
}

void SceneParametricCurve::onPoint1Changed(glm::vec3& point) {
    setPoint(1, point);
    build();
}

void SceneParametricCurve::onPoint2Changed(glm::vec3& point) {
    setPoint(2, point);
    build();
}

void SceneParametricCurve::onPoint3Changed(glm::vec3& point) {
    setPoint(3, point);
    build();
}

void SceneParametricCurve::onPoint4Changed(glm::vec3& point) {
    setPoint(4, point);
    build();
}

void SceneParametricCurve::draw() {
    ofPushMatrix();
    ofMultMatrix(node->getGlobalTransformMatrix());
    curve.draw();
    ofPopMatrix();
}

void SceneParametricCurve::drawGizmo() const {
    ofPushMatrix();
    ofMultMatrix(node->getGlobalTransformMatrix());
    t1.draw();
    t2.draw();
    ofPopMatrix();

    SceneElement::drawGizmo();
}

}  // namespace engine
