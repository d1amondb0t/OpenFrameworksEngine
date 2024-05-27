#include "image_configuration.h"

namespace engine {

void ImageConfiguration::setup(std::shared_ptr<ImageDrawingTool> imageDrawingTool) {
    this->imageDrawingTool = imageDrawingTool;
}

void ImageConfiguration::importImage() {
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a .png or .jpg file");

    if (openFileResult.bSuccess) {
        ofFile file(openFileResult.getPath());

        if (file.exists()) {
            string fileExtension = ofToUpper(file.getExtension());
            if (fileExtension == "JPG" || fileExtension == "PNG") {
                ofImage image;
                image.load(openFileResult.getPath());

                imageDrawingTool->setImage(image);
            }
        }
    }
}

void ImageConfiguration::startCapture(const CaptureSettings& settings) {
    captureStartTime = ofGetElapsedTimef();
    captureEndTime = captureStartTime + settings.duration;
    fileExtension = settings.fileExtension;
    filePrefix = settings.framePrefix;

    captureInProgress = true;
}

bool ImageConfiguration::isCapturing() {
    if (captureInProgress && ofGetElapsedTimef() > captureEndTime) {
        captureInProgress = false;

        saveFramesToDisk();
    }

    return captureInProgress;
}

void ImageConfiguration::saveFrame(const ofFbo& framebuffer) {
    ofFbo fbo;
    fbo.allocate(framebuffer.getWidth(), framebuffer.getHeight());

    fbo.begin();
    framebuffer.draw(0, 0);
    fbo.end();

    cachedFrames.push_back(fbo);
}

void ImageConfiguration::saveFramesToDisk() {
    int cpt = 0;
    for (auto frame : cachedFrames) {
        ofPixels pixels;
        frame.readToPixels(pixels);

        ofImage image(pixels);
        image.save(filePrefix + std::to_string(++cpt) + "." + fileExtension);
    }

    cachedFrames.clear();
}

}  // namespace engine