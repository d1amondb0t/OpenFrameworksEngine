#include "scene_3d_model.h"

namespace engine {

Scene3DModel::Scene3DModel(
    uint32_t id, std::string name, const std::string modelPath, Material* material, SceneElement* parent)
    : Scene3DElement(id, name, new ofNode(), material, parent) {
    model.load(modelPath, ofxAssimpModelLoader::OPTIMIZE_DEFAULT);

    loadModelTangents();

    model.disableMaterials();
    model.setScale(1.f, -1.f, 1.f);
}

Scene3DModel::~Scene3DModel() {
    delete[] tangents;
    delete[] bitangents;
}

void Scene3DModel::draw() {
    ofPushMatrix();

    ofMultMatrix(node->getGlobalTransformMatrix());

    material->begin();

    model.drawFaces();

    material->end();

    ofPopMatrix();
}

void Scene3DModel::loadModelTangents() {
    if (model.getMeshCount() == 0) return;

    ofxAssimpMeshHelper& helper = model.getMeshHelper(0);
    aiMesh* aim = helper.mesh;

    if (aim->HasTangentsAndBitangents()) {
        int tangentLocation = 4;
        int bitangentLocation = 5;

        int nVertices = aim->mNumVertices;
        tangents = new float[nVertices * 3];
        bitangents = new float[nVertices * 3];

        for (int i = 0; i < (int)aim->mNumVertices; i++) {
            tangents[i * 3] = aim->mTangents[i].x;
            tangents[3 * i + 1] = aim->mTangents[i].y;
            tangents[3 * i + 2] = aim->mTangents[i].z;

            bitangents[i * 3] = aim->mBitangents[i].x;
            bitangents[3 * i + 1] = aim->mBitangents[i].y;
            bitangents[3 * i + 2] = aim->mBitangents[i].z;
        }

        helper.vbo.setAttributeData(tangentLocation, tangents, 3, nVertices, GL_STATIC_DRAW, sizeof(float) * 3);
        helper.vbo.setAttributeData(bitangentLocation, bitangents, 3, nVertices, GL_STATIC_DRAW, sizeof(float) * 3);
    }
}

}  // namespace engine