#include "image_histogram.h"

#include <vector>

ImageHistogram::ImageHistogram() {
    set(0, 0, 0, 0);
}

ImageHistogram::ImageHistogram(float px, float py, float width, float height) {
    set(px, py, width, height);
    isVisible = false;
}

float ImageHistogram::getX() const {
    return x;
}

float ImageHistogram::getY() const {
    return y;
}

float ImageHistogram::getWidth() const {
    return width;
}

float ImageHistogram::getHeight() const {
    return height;
}

void ImageHistogram::set(float px, float py, float width, float height) {
    setPosition(px, py);
    resize(width, height);
}

void ImageHistogram::setPosition(float px, float py) {
    this->x = px;
    this->y = py;
}

void ImageHistogram::resize(float width, float height) {
    float dw = width / this->width;
    float dh = height / this->height;

    this->width = width;
    this->height = height;

    for (auto &bar : bars) {
        bar.scale(dw, dh);
    }
}

void ImageHistogram::setVisible(bool isVisible) {
    this->isVisible = isVisible;
}

void ImageHistogram::load(const ofImage &image) {
    this->image = &image;

    switch (image.getImageType()) {
        case OF_IMAGE_GRAYSCALE:
            loadGrayscaleImage(image);
            break;

        case OF_IMAGE_COLOR:
            loadRGBImage(image);
            break;

        case OF_IMAGE_COLOR_ALPHA:
            loadRGBImage(image);
            break;

        default:
            ofLogError() << "Unknown image type; cannot load image histogram.";
            break;
    };
}

void ImageHistogram::draw() const {
    if (!isVisible) return;

    for (auto bar : bars) {
        bar.draw(x, y);
    }
}

void ImageHistogram::loadGrayscaleImage(const ofImage &image) {
    ofPixels pixels = image.getPixels();
    std::vector<int> frequencies(BRIGHTNESS_COUNT, 0);

    for (auto pixel : pixels) {
        frequencies[pixel]++;
    }

    extractSingleColorRectangles(frequencies, ofColor::black);
}

void ImageHistogram::loadRGBImage(const ofImage &image) {
    ofPixels pixels = image.getPixels();
    size_t componentsCount = pixels.getNumChannels();

    std::vector<int> red(BRIGHTNESS_COUNT, 0);
    std::vector<int> green(BRIGHTNESS_COUNT, 0);
    std::vector<int> blue(BRIGHTNESS_COUNT, 0);

    int cpt = 0;
    for (auto pixel : pixels) {
        switch (cpt % componentsCount) {
            case R_COMPONENT_INDEX:
                red[pixel]++;
                break;

            case G_COMPONENT_INDEX:
                green[pixel]++;
                break;

            case B_COMPONENT_INDEX:
                blue[pixel]++;
                break;

            default:
                break;
        }

        cpt++;
    }

    extractRGBRectangles(red, green, blue);
}

struct ColorFrequency {
    ofColor color;
    int frequency;

    ColorFrequency(ofColor color, int frequency) : color(color), frequency(frequency) {}

    bool operator<(const ColorFrequency &rhs) { return this->frequency < rhs.frequency; }
};

void ImageHistogram::extractRGBRectangles(std::vector<int> redFrequencies,
                                          std::vector<int> greenFrequencies,
                                          std::vector<int> blueFrequencies) {
    size_t layerSize = redFrequencies.size();
    size_t layerCount = 3;  // R + B + G

    bars.clear();
    bars.reserve(layerSize * layerCount);

    int maximumFrequency = 0;
    maximumFrequency = max(maximumFrequency, *std::max_element(redFrequencies.begin(), redFrequencies.end()));
    maximumFrequency = max(maximumFrequency, *std::max_element(greenFrequencies.begin(), greenFrequencies.end()));
    maximumFrequency = max(maximumFrequency, *std::max_element(blueFrequencies.begin(), blueFrequencies.end()));

    float heightToFrequencyRatio = height / maximumFrequency;
    float barWidth = width / layerSize;

    float y = 0;
    std::vector<ColorFrequency> frequencyOrder;
    for (size_t i = 0; i < layerSize; ++i) {
        frequencyOrder.push_back(ColorFrequency(ofColor::red, redFrequencies[i]));
        frequencyOrder.push_back(ColorFrequency(ofColor::green, greenFrequencies[i]));
        frequencyOrder.push_back(ColorFrequency(ofColor::blue, blueFrequencies[i]));

        std::sort(frequencyOrder.begin(), frequencyOrder.end());

        ofColor currentColor(0, 0, 0);

        y = (maximumFrequency - frequencyOrder.back().frequency) * heightToFrequencyRatio;
        for (int j = frequencyOrder.size() - 1; j >= 0; j--) {
            int nextFrequency = j > 0 ? frequencyOrder[j - 1].frequency : 0;

            currentColor += frequencyOrder[j].color;

            float rectangleHeight = (frequencyOrder[j].frequency - nextFrequency) * heightToFrequencyRatio;

            if (rectangleHeight > 0) {
                ofPath rectangle;
                rectangle.rectangle(i * barWidth, y, barWidth, rectangleHeight);
                rectangle.setColor(currentColor);

                bars.push_back(rectangle);
            }

            y += rectangleHeight;
        }

        frequencyOrder.clear();
    }
}

void ImageHistogram::extractSingleColorRectangles(std::vector<int> frequencies, ofColor color) {
    bars.clear();
    bars.reserve(frequencies.size());

    int maximumFrequency = *std::max_element(frequencies.begin(), frequencies.end());
    float heightToFrequencyRatio = height / maximumFrequency;
    float barWidth = width / frequencies.size();

    for (size_t i = 0; i < frequencies.size(); i++) {
        int frequency = frequencies[i];
        if (frequency == 0) continue;

        float y = (maximumFrequency - frequency) * heightToFrequencyRatio;
        float rectangleHeight = frequency * heightToFrequencyRatio;

        ofPath rectangle;
        rectangle.rectangle(i * barWidth, y, barWidth, rectangleHeight);
        rectangle.setColor(color);

        bars.push_back(rectangle);
    }
}
