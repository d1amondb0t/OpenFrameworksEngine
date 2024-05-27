#include "histogram_wrapper.h"

engine::HistogramWrapper::HistogramWrapper() {
    histogram.set(0, 0, FRAMEBUFFER_RESOLUTION, FRAMEBUFFER_RESOLUTION);
    histogram.setVisible(true);

    framebuffer.allocate(FRAMEBUFFER_RESOLUTION, FRAMEBUFFER_RESOLUTION, GL_RGBA);

    framebuffer.begin();
    ofClear(50);
    framebuffer.end();

    framebuffer.readToPixels(pixels);
}

ofPixels& engine::HistogramWrapper::getPixels() {
    return pixels;
}

void engine::HistogramWrapper::refresh(const ofFbo& lastFrameBuffer) {
    ofPixels lastFramePixels;
    lastFrameBuffer.readToPixels(lastFramePixels);
    ofImage lastFrame(lastFramePixels);

    histogram.load(lastFrame);

    framebuffer.begin();
    {
        ofClear(50);

        histogram.draw();
    }
    framebuffer.end();

    framebuffer.readToPixels(pixels);
}
