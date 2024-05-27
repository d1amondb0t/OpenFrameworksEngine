#pragma once

#include "image_histogram.h"

namespace engine {

class HistogramWrapper {
    static constexpr int FRAMEBUFFER_RESOLUTION = 512;

  public:
    HistogramWrapper();

    ofPixels& getPixels();

    void refresh(const ofFbo& lastFrameBuffer);

  private:
    ImageHistogram histogram;
    ofFbo framebuffer;
    ofPixels pixels;
};

}  // namespace engine
