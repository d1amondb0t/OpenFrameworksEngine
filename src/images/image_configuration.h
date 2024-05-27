#pragma once

#include <list>
#include <memory>

#include "../tools/image_drawing_tool.h"
#include "ofMain.h"

namespace engine {
struct CaptureSettings {
    std::string framePrefix;
    float duration;
    std::string fileExtension;
};

class ImageConfiguration {
  public:
    void setup(std::shared_ptr<ImageDrawingTool> imageDrawingTool);

    void importImage();

    bool isCapturing();
    void startCapture(const CaptureSettings& settings);
    void saveFrame(const ofFbo& framebuffer);
    void saveFramesToDisk();

  private:
    std::vector<ofImage> imagesLoaded;
    std::vector<ofImage> imagesProcessed;
    ofImage captureScene;

    std::shared_ptr<ImageDrawingTool> imageDrawingTool;

    bool captureInProgress = false;
    float captureStartTime = 0;
    float captureEndTime = 0;
    std::string filePrefix = "";
    std::string fileExtension = "png";

    std::list<ofFbo> cachedFrames;
};

}  // namespace engine
