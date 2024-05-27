#pragma once

#include <vector>
#include "../vector_graphics/image.hpp"
#include "ImHelpers.h"
#include "ofMain.h"
#include "ofxImGui.h"
#include <algorithm>

namespace engine {

    enum ColorModes { COLOR, GREY };   

    // kernel de convolution (3x3) : identité
    const std::array<float, 9> convolution_kernel_identity{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    // kernel de convolution (3x3) : aiguiser
    const std::array<float, 9> convolution_kernel_sharpen{0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f, 0.0f};
    // kernel de convolution (3x3) : détection de bordure
    const std::array<float, 9> convolution_kernel_edge_detect{0.0f, 1.0f, 0.0f, 1.0f, -4.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    // kernel de convolution (3x3) : bosseler
    const std::array<float, 9> convolution_kernel_emboss{-2.0f, -1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f};
    // kernel de convolution (3x3) : flou
    const std::array<float, 9> convolution_kernel_blur{1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f};

class ImageFilter {
  public:
    ofImage defaultImage;
    ofImage imageFiltered;

    ImageFilter();
    void prepare(ofxImGui::Settings &settings);
    void setDefaultImage(ofImage &image);
    ofImage getFinalImage();
    void resetImageFilter();
    void greyImage();
    void hadModification();
    void invertImageColor();
    void colorFilter(ofColor &color, float intensity);
    void changeExposure();
    void changeGamma();
    void acesFilmicToneMapping();
    void reinhardToneMapping();

    void onModeChanged(int &value);
    void onColorMixedChanged(float &value);
    void onExposureChanged(float &value);
    void onGammaChanged(float &value);
    void convolutionfilter(int value);

  private:
    ofParameter<int> selectedMode;
    ofParameter<float> exposure;
    ofParameter<float> gamma;
    ofParameter<float> colorMixer;
    ofParameter<ofFloatColor> selectedColor;
    ofTexture texture;
    ofColor finalColor;
    ofImage ImageModification;
    int imageWidth;
    int imageHeight;
    bool toneMappingToggle;
    bool toneMappingActive;
    bool imageLoaded;
    bool isInModification;
};

}  // namespace engine
