#include "image_filter.h"

namespace engine {

ImageFilter::ImageFilter() {
    selectedMode.set("Selected Mode", ColorModes::COLOR, ColorModes::COLOR, ColorModes::GREY);
    selectedMode.addListener(this, &ImageFilter::onModeChanged);
    colorMixer.set("Color Mixer", 0, 0, 1.0f);
    colorMixer.addListener(this, &ImageFilter::onColorMixedChanged);
    exposure.set("Exposure", 1.2, 0, 5.0f);
    exposure.addListener(this, &ImageFilter::onExposureChanged);
    gamma.set("Gamma Correction", 1, 0, 5.0f);
    gamma.addListener(this, &ImageFilter::onGammaChanged);

    imageLoaded = false;
    imageWidth = 0;
    imageHeight = 0;
    exposure = 1.2;
    gamma = 1;
    toneMappingToggle = true;
    toneMappingActive = false;
    isInModification = false;
}
    
void ImageFilter::prepare(ofxImGui::Settings &settings) {
    ofxImGui::BeginWindow("Image Filter", settings);

    if (imageLoaded) {
        ofxImGui::AddRadio(selectedMode, {"Color", "Grey"});
        ofFloatColor temp = selectedColor.get();

        if (ImGui::Button("Reset Filter")) resetImageFilter();

        if (ImGui::ColorEdit3("Color", &temp.r)) {
            selectedColor.set(temp);
            int r = temp.r * 255;
            int g = temp.g * 255;
            int b = temp.b * 255;
            finalColor = ofColor(r, g, b);
            colorFilter(finalColor, colorMixer);
        }

        ofxImGui::AddSlider(colorMixer);

        if (!toneMappingActive) {
            if (ImGui::Button("Tone mapping Off")) {
                toneMappingActive = true;
                reinhardToneMapping();
            }
        } else {
            if (ImGui::Button("Tone Mapping On")) {
                resetImageFilter();
            }
            ofxImGui::AddSlider(exposure);
            ofxImGui::AddSlider(gamma);

            if (!toneMappingToggle) {
                if (ImGui::Button("Reinhard active")) acesFilmicToneMapping();
            } else {
                if (ImGui::Button("ACES filmic active")) reinhardToneMapping();
            }
        }

        if (!imageFiltered.getImageType() == OF_IMAGE_GRAYSCALE) {
            if (ImGui::Button("Convolution Identity")) convolutionfilter(0);
            if (ImGui::Button("Convolution Sharpen")) convolutionfilter(1);
            if (ImGui::Button("Convolution Edge Detect")) convolutionfilter(2);
            if (ImGui::Button("Convolution Emboss")) convolutionfilter(3);
            if (ImGui::Button("Convolution blur")) convolutionfilter(4);
        }

        if (ImGui::Button("Inverse Color")) invertImageColor();

        auto size = ImGui::GetContentRegionAvail();
        ImGui::Image(GetImTextureID(imageFiltered.getTexture()), {size.x, size.y});

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && isInModification) {
            isInModification = false;
            hadModification();
        }

    } else {
        ImGui::Text("No image loaded.");
    }

    ofxImGui::EndWindow(settings);
}

void ImageFilter::setDefaultImage(ofImage &image) {
    defaultImage = image;
    imageFiltered = defaultImage;
    imageLoaded = true;
    imageWidth = defaultImage.getWidth();
    imageHeight = defaultImage.getHeight();
    hadModification();
}

ofImage ImageFilter::getFinalImage() {
    return imageFiltered;
}

void ImageFilter::resetImageFilter() {
    imageFiltered = defaultImage;
    ImageModification = defaultImage;
    selectedMode = 0;
    colorMixer = 0;
    selectedColor = (1, 1, 1);
    exposure = 1.2;
    gamma = 1;
    toneMappingToggle = true;
    toneMappingActive = false;
}

void ImageFilter::hadModification() {
    ImageModification = imageFiltered;
}

void ImageFilter::greyImage() {
    imageFiltered.setImageType(OF_IMAGE_GRAYSCALE);
    hadModification();
}

void ImageFilter::invertImageColor() {
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            ofColor color = imageFiltered.getColor(x, y);

            int invertedR = 255 - color.r;
            int invertedG = 255 - color.g;
            int invertedB = 255 - color.b;

            imageFiltered.setColor(x, y, ofColor(invertedR, invertedG, invertedB));
        }
    }
    imageFiltered.update();
    hadModification();
}

void ImageFilter::colorFilter(ofColor &color, float intensity) {
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            ofColor pixelColor = ImageModification.getColor(x, y);
           
            pixelColor.r = pixelColor.r * (1 - intensity) + color.r * intensity;
            pixelColor.g = pixelColor.g * (1 - intensity) + color.g * intensity;
            pixelColor.b = pixelColor.b * (1 - intensity) + color.b * intensity;
            
            imageFiltered.setColor(x, y, pixelColor);
        }
    }
    imageFiltered.update();
}

void ImageFilter::changeExposure() {
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            ofColor pixelColor = ImageModification.getColor(x, y);
            float r = pixelColor.r / 255.0;
            float g = pixelColor.g / 255.0;
            float b = pixelColor.b / 255.0;

            pixelColor.r = (1.0 - exp(-r * exposure)) * 255;
            pixelColor.g = (1.0 - exp(-g * exposure)) * 255;
            pixelColor.b = (1.0 - exp(-b * exposure)) * 255;

            imageFiltered.setColor(x, y, pixelColor);
        }
    }
    imageFiltered.update();
}

void ImageFilter::changeGamma() {
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            ofColor pixelColor = ImageModification.getColor(x, y);
            float r = pixelColor.r / 255.0;
            float g = pixelColor.g / 255.0;
            float b = pixelColor.b / 255.0;

            pixelColor.r = (pow(r, (1.0 / gamma))) * 255;
            pixelColor.g = (pow(g, (1.0 / gamma))) * 255;
            pixelColor.b = (pow(b, (1.0 / gamma))) * 255;

            imageFiltered.setColor(x, y, pixelColor);
        }
    }
    imageFiltered.update();
}

void ImageFilter::reinhardToneMapping() {
    if (toneMappingToggle) {
        for (int y = 0; y < imageHeight; ++y) {
            for (int x = 0; x < imageWidth; ++x) {
                ofColor pixelColor = ImageModification.getColor(x, y);
                float r = pixelColor.r / 255.0;
                float g = pixelColor.g / 255.0;
                float b = pixelColor.b / 255.0;

                pixelColor.r = (r / (r + 1.0)) * 255;
                pixelColor.g = (g / (g + 1.0)) * 255;
                pixelColor.b = (b / (b + 1.0)) * 255;

                imageFiltered.setColor(x, y, pixelColor);
            }
        }
        imageFiltered.update();
        hadModification();
        toneMappingToggle = !toneMappingToggle;
    }
}

void ImageFilter::acesFilmicToneMapping() {
     if (!toneMappingToggle) {
        float a = 2.51f;
        float b = 0.03f;
        float c = 2.43f;
        float d = 0.59f;
        float e = 0.14f;

        for (int y = 0; y < imageHeight; ++y) {
            for (int x = 0; x < imageWidth; ++x) {
                ofColor pixelColor = ImageModification.getColor(x, y);
                float cR = pixelColor.r / 255.0;
                float cG = pixelColor.g / 255.0;
                float cB = pixelColor.b / 255.0;
                pixelColor.r = std::clamp((cR * (a * cR + b)) / (cR * (c * cR + d) + e), 0.0f, 1.0f) * 255;
                pixelColor.g = std::clamp((cG * (a * cG + b)) / (cG * (c * cG + d) + e), 0.0f, 1.0f) * 255;
                pixelColor.b = std::clamp((cB * (a * cB + b)) / (cB * (c * cB + d) + e), 0.0f, 1.0f) * 255;

                imageFiltered.setColor(x, y, pixelColor);
            }
        }
        imageFiltered.update();
        hadModification();
        toneMappingToggle = !toneMappingToggle;
    }
}

void ImageFilter::onModeChanged(int &value) {
    if (value == 0) {
        if (!imageFiltered.getImageType() == OF_IMAGE_COLOR) {
            resetImageFilter();
        }
    } else {
        if (!imageFiltered.getImageType() == OF_IMAGE_GRAYSCALE) {
            greyImage();
        }
    }
}

void ImageFilter::onColorMixedChanged(float &value) 
{
    colorMixer = value;
    isInModification = true;
    colorFilter(finalColor, colorMixer);
}

void ImageFilter::onExposureChanged(float &value) {
    exposure = value;
    isInModification = true;
    changeExposure();
}

void ImageFilter::onGammaChanged(float &value) {
    gamma = value;
    isInModification = true;
    changeGamma();
}

void ImageFilter::convolutionfilter(int value) {
    const int kernel_size = 3;
    const int kernel_offset = kernel_size / 2;
    const int color_component_count = 3;
    int x, y;
    int i, j;
    int xi, yj;
    int c;
    int pixel_index_img_src;
    int pixel_index_img_dst;
    int kernel_index;
    float kernel_value;

    // extraire les pixels de l'image source
    ofPixels pixel_array_src = ImageModification.getPixels();
    // extraire les pixels de l'image de destination
    ofPixels pixel_array_dst = imageFiltered.getPixels();

    // couleur du pixel lu dans l'image source
    ofColor pixel_color_src;

    // couleur du pixel à écrire dans l'image de destination
    ofColor pixel_color_dst;

    // somme du kernel appliquée à chaque composante de couleur d'un pixel
    float sum[3];

    // itération sur les rangées des pixels de l'image source
    for (y = 0; y < imageHeight; ++y) {
        // itération sur les colonnes des pixels de l'image source
        for (x = 0; x < imageWidth; ++x) {
            // initialiser le tableau où les valeurs de filtrage sont accumulées
            for (c = 0; c < color_component_count; ++c) sum[c] = 0;

            // déterminer l'index du pixel de l'image de destination
            pixel_index_img_dst = (imageWidth * y + x) * color_component_count;

            // itération sur les colonnes du kernel de convolution
            for (j = -kernel_offset; j <= kernel_offset; ++j) {
                // itération sur les rangées du kernel de convolution
                for (i = -kernel_offset; i <= kernel_offset; ++i) {
                    // calculer les indices d'échantillonnage
                    xi = x - i;
                    yj = y - j;

                    // traitement de la bordure
                    if (xi < 0 || xi >= imageWidth || yj < 0 || yj >= imageHeight) continue;

                    // déterminer l'index du pixel de l'image source à lire
                    pixel_index_img_src = (imageWidth * yj + xi) * color_component_count;

                    // lire la couleur du pixel de l'image source
                    pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);

                    // déterminer l'indice du facteur à lire dans le kernel de convolution
                    kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);

                    // extraction de la valeur à cet index du kernel
                    switch (value) {
                        case 0:
                            kernel_value = convolution_kernel_identity.at(kernel_index);
                            break;

                        case 1:
                            kernel_value = convolution_kernel_emboss.at(kernel_index);
                            break;

                        case 2:
                            kernel_value = convolution_kernel_sharpen.at(kernel_index);
                            break;

                        case 3:
                            kernel_value = convolution_kernel_edge_detect.at(kernel_index);
                            break;

                        case 4:
                            kernel_value = convolution_kernel_blur.at(kernel_index);
                            break;

                        default:
                            kernel_value = convolution_kernel_identity.at(kernel_index);
                            break;
                    }

                    // itération sur les composantes de couleur
                    for (c = 0; c < color_component_count; ++c) {
                        // accumuler les valeurs de filtrage en fonction du kernel de convolution
                        sum[c] = sum[c] + kernel_value * pixel_color_src[c];
                    }
                }
            }

            // déterminer la couleur du pixel à partir des valeurs de filtrage accumulées pour chaque composante
            for (c = 0; c < color_component_count; ++c) {
                // conversion vers entier et validation des bornes de l'espace de couleur
                pixel_color_dst[c] = (int)ofClamp(sum[c], 0, 255);
            }

            // écrire la couleur à l'index du pixel en cours de filtrage
            pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
        }
    }

    // écrire les pixels dans l'image de destination
    imageFiltered.setFromPixels(pixel_array_dst);
    imageFiltered.update();
    hadModification();
}

}  // namespace engine
