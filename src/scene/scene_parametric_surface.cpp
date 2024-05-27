#include "scene_parametric_surface.h"

namespace engine {

SceneParametricSurface::SceneParametricSurface(uint32_t id, std::string name, Material* material, SceneElement* parent)
    : SceneParametricCurve(id, name, material, parent, 4), surface() {
    surface.setMode(OF_PRIMITIVE_LINES);

    curvePoints.push_back(glm::vec3(-90.0f, 0.0f, -90.0f));
    curvePoints.push_back(glm::vec3(-70.0f, 100.0f, -90.0f));
    curvePoints.push_back(glm::vec3(70.0f, 100.0f, -90.0f));
    curvePoints.push_back(glm::vec3(90.0f, 0.0f, -90.0f));

    curvePoints.push_back(glm::vec3(90.0f, 100.0f, -70.0f));
    curvePoints.push_back(glm::vec3(90.0f, 100.0f, 70.0f));
    curvePoints.push_back(glm::vec3(90.0f, 0.0f, 90.0f));

    curvePoints.push_back(glm::vec3(70.0f, 100.0f, 90.0f));
    curvePoints.push_back(glm::vec3(-70.0f, 100.0f, 90.0f));
    curvePoints.push_back(glm::vec3(-90.0f, 0.0f, 90.0f));

    curvePoints.push_back(glm::vec3(-90.0f, 100.0f, 70.0f));
    curvePoints.push_back(glm::vec3(-90.0f, 100.0f, -70.0f));
    curvePoints.push_back(glm::vec3(-90.0f, 0.0f, -90.0f));

    p1 = &curvePoints.at(0);
    p2 = &curvePoints.at(1);
    p3 = &curvePoints.at(2);
    p4 = &curvePoints.at(3);
    build();
}

SceneParametricSurface::~SceneParametricSurface() {}

long SceneParametricSurface::factorial(const int n) {
    long f = 1;
    for (int i = 1; i <= n; ++i) f *= i;
    return f;
}

int SceneParametricSurface::bernstein(long n, long k, long t) {
    long nMinusK = n - k;

    long b = (factorial(n) / (factorial(k) * factorial(nMinusK))) * pow(t, k) * pow((t - 1), nMinusK);

    return b;
}

glm::vec3 SceneParametricSurface::blerp(double u, double v) {
    return (((1 - u) * (1 - v) * curvePoints.at(0)) + (u * (1 - v) * curvePoints.at(9)) + ((1 - u) * v * curvePoints.at(3)) + (u * v * curvePoints.at(6)));
}

glm::vec3 SceneParametricSurface::lerpu(double u, double v) {
    return (((1 - v) * SceneParametricCurve::buildVertex(1 - u, 9)) + (v * SceneParametricCurve::buildVertex(u, 3)));
}

glm::vec3 SceneParametricSurface::lerpv(double u, double v) {
    return (((1 - u) * SceneParametricCurve::buildVertex(v, 0)) + (u * SceneParametricCurve::buildVertex(1 - v, 6)));
}

// La fonction utilisée est celle de la surface de Coons, mais comme l'ordre des points n'est pas exactement comme dans les notes de cours, le patchage ne donne pas un résultat concluant.
// La surface paramétrique ne fonctionne donc pas.
glm::vec3 SceneParametricSurface::buildVertex(double u, double v) {
    return lerpu(u, v) + lerpv(u, v) - blerp(u, v);
}

void SceneParametricSurface::build() {
    surface.clear();

    double fraction = 1.0 / subdivisionCount;
    double u = 0.0;
    double v = 0.0;
    glm::vec3 vertex;

    for (double i = 0; i <= subdivisionCount; i++, u += fraction) {
        for (double j = 0; j <= subdivisionCount; j++, v += fraction) {
            vertex = buildVertex(u, v);
            surface.addVertex(vertex);
        }
    }
}

ofParameterGroup SceneParametricSurface::getAttributes(bool isMultiSelect) {
    ofParameterGroup parameters = SceneParametricCurve::getAttributes(isMultiSelect);

    parameters.remove("Segment Count");

    return parameters;
}

void SceneParametricSurface::draw() {
    ofPushMatrix();
    ofMultMatrix(node->getGlobalTransformMatrix());
    surface.draw();
    ofPopMatrix();
}

} // namespace engine
