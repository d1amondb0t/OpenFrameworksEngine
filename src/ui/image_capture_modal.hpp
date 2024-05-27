#pragma once

#include <vector>

#include "../images/image_configuration.h"
#include "ofMain.h"
#include "ofxImGui.h"

namespace engine {

class ImageCaptureModal {
  public:
    ofEvent<CaptureSettings> onCaptureStart;

    void prepare() {
        if (ImGui::BeginPopupModal("Image export")) {
            ImGui::InputText("Prefix", framePrefix, 255);
            ImGui::SliderFloat("Duration (seconds)", &duration, 0.1f, 5.0f);
            for (size_t i = 0; i < extensions.size(); i++) {
                ImGui::RadioButton(extensions[i].c_str(), &selectedExtension, i);
            }

            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Start")) {
                CaptureSettings settings;
                settings.framePrefix = framePrefix;
                settings.duration = duration;
                settings.fileExtension = extensions[selectedExtension];

                ofNotifyEvent(onCaptureStart, settings);

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

  private:
    char framePrefix[255] = "";
    std::vector<std::string> extensions = {"png", "jpg"};
    int selectedExtension = 0;
    float duration = 0;
};

}  // namespace engine
