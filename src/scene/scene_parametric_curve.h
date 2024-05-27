#pragma once

#include <vector>

#include "scene_3d_element.h"

namespace engine {

class SceneParametricCurve : public Scene3DElement {
  public:
    SceneParametricCurve(uint32_t id, std::string name, Material* material, SceneElement* parent = nullptr, int segmentCount = 1);

    virtual ~SceneParametricCurve();

    glm::vec3 buildVertex(double t, int startIndex);
    void build();
    void buildSegment(int startIndex);

    glm::vec3 getPoint(int index) const;

    void setPoint(int index, glm::vec3 point);

    virtual ofParameterGroup getAttributes(bool isMultiSelect) override;
    virtual void listen(ofParameterGroup attributesToListen, bool isMultiSelect) override;

    void onSubdivisionCountChanged(int& subdivisionCount);

    void onSegmentCountChanged(int& segmentCount);

    void onCurrentSegmentChanged(int& point);

    void onPoint1Changed(glm::vec3& point);
    void onPoint2Changed(glm::vec3& point);
    void onPoint3Changed(glm::vec3& point);
    void onPoint4Changed(glm::vec3& point);

    void draw() override;

    void drawGizmo() const override;

  protected:
    vector<glm::vec3> curvePoints;
    glm::vec3* p1;
    glm::vec3* p2;
    glm::vec3* p3;
    glm::vec3* p4;
    int subdivisionCount;
    int segmentCount;
    int currentModifiedSegment;
    ofPolyline curve;
    ofPolyline t1;
    ofPolyline t2;
};

}  // namespace engine
