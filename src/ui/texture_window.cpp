#include "texture_window.h"

namespace engine {

TextureWindow::TextureWindow() : textureGenerator(2048) {
    ofDisableArbTex();

    generationTypeLabels = {"Checkers", "Bricks", "Noise"};
    proceduralGenerationType.set("Procedural type", CHECKERS, CHECKERS, NOISE);

    checkersCount.set("Count", 8, 1, 64);
    evenCheckersColor.set("Even Color", ofColor::white, ofColor(0.0f), ofColor(255.0f));
    oddCheckersColor.set("Odd Color", ofColor::black, ofColor(0.0f), ofColor(255.0f));

    bricksWidth.set("Brick Width", 256, 16, 2048);
    bricksHeight.set("Brick Height", 64, 8, 2048);
    brickColor.set("Brick Color", ofColor::fireBrick, ofColor(0.0f), ofColor(255.0f));
    marginSize.set("Margin Size", 16, 2, 512);
    marginColor.set("Margin Color", ofColor::gray, ofColor(0.0f), ofColor(255.0f));

    frequency.set("Frequency", 2.0f, 0.1f, 1024.0f);
    midpoint.set("Midpoint", 0.5f, 0.0f, 1.0f);
    mixEdgeSize.set("Mix Edge Size", 0.02f, 0.f, 1.0f);
    highValuesColor.set("High Values Color", ofColor::red, ofColor(0.0f), ofColor(255.0f));
    lowValuesColor.set("Low Values Color", ofColor::blue, ofColor(0.0f), ofColor(255.0f));

    for (int i = 0; i < 3; ++i) {
        std::shared_ptr<ofTexture> texture = std::make_shared<ofTexture>();
        texture->allocate(2048, 2048, GL_RGBA);
        textures.push_back(texture);
    }
}

void TextureWindow::prepare(ofxImGui::Settings& settings) {
    ofxImGui::BeginWindow("Textures", settings);

    ofxImGui::AddRadio(proceduralGenerationType, generationTypeLabels, 3);

    ofParameterGroup parameters;
    parameters.setName("Parameters");
    switch (proceduralGenerationType.get()) {
        case CHECKERS:
            parameters.add(checkersCount);
            parameters.add(evenCheckersColor);
            parameters.add(oddCheckersColor);
            break;

        case BRICKS:
            parameters.add(bricksWidth);
            parameters.add(bricksHeight);
            parameters.add(brickColor);
            parameters.add(marginSize);
            parameters.add(marginColor);
            break;

        case NOISE:
            parameters.add(frequency);
            parameters.add(midpoint);
            parameters.add(mixEdgeSize);
            parameters.add(highValuesColor);
            parameters.add(lowValuesColor);
            break;
    }

    ofxImGui::AddGroup(parameters, settings);

    if (ImGui::Button("Generate")) {
        generateTexture();
    }
    ImGui::SameLine();
    ImGui::Button("Drag and drop");

    ofTexture& currentTexture = *textures.at(proceduralGenerationType.get());
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(TEXTURE_PAYLOAD.c_str(), &currentTexture, sizeof(currentTexture));

        ImGui::Text("Texture");

        ImGui::EndDragDropSource();
    }

    auto size = ImGui::GetContentRegionAvail();
    ImGui::Image(GetImTextureID(currentTexture), {size.x, size.y});

    ofxImGui::EndWindow(settings);
}
void TextureWindow::generateTexture() {
    switch (proceduralGenerationType.get()) {
        case CHECKERS:
            textureGenerator.generateCheckerTexture(
                *textures.at(CHECKERS), checkersCount, evenCheckersColor, oddCheckersColor);
            break;

        case BRICKS:
            textureGenerator.generateBricksTexture(
                *textures.at(BRICKS), bricksWidth, bricksHeight, marginSize, brickColor, marginColor);
            break;

        case NOISE:
            textureGenerator.generatePerlinNoiseTexture(
                *textures.at(NOISE), frequency, midpoint, mixEdgeSize, highValuesColor, lowValuesColor);
            break;
    }
}
}  // namespace engine