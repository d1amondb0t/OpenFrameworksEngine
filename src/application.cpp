#include "application.h"

const float SPEED = 1000.f;

namespace engine {
Application::Application(std::shared_ptr<Scene> scene,
                         std::shared_ptr<MaterialCatalog> materialCatalog,
                         std::shared_ptr<CameraController> cameraControl,
                         std::shared_ptr<InputController> _inputController)
    : sceneGraphWindow(scene, materialCatalog),
      elementAttributeWindow(scene),
      materialWindow(materialCatalog),
      renderer(scene, cameraControl),
      cameraWindow(cameraControl),
      inputController(_inputController) {

    valuesToChange[0] = valuesToChange[1] = nullptr;
    imageFilter = std::make_shared<ImageFilter>();
    for (const auto window : Window::All) {
        shownWindows.insert(std::pair<const Window::Type, bool>(window, true));
    }
    shownWindows[Window::HISTOGRAM] = false;

    gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true);

    ofAddListener(imageCaptureModal.onCaptureStart, this, &Application::onExportImagesButton);
}

Application::~Application() {
    ofRemoveListener(imageCaptureModal.onCaptureStart, this, &Application::onExportImagesButton);
}

void Application::setup() {
    currentWindowSize = ofGetWindowSize();
    ofSetBackgroundAuto(false);
    ofSetCircleResolution(64);
    updateLogLevel();

    renderingFBO.allocate(currentWindowSize.x, currentWindowSize.y, GL_RGBA);
    renderer.setup(inputController, imageFilter);
    renderer.toggleOrigin(showOrigin);
    imageConfig.setup(std::dynamic_pointer_cast<ImageDrawingTool>(renderer.getDrawingTool(VectorDrawMode::IMAGE)));

    currentDrawingToolParameters = renderer.getDrawingTool()->getParameters();
}

//--------------------------------------------------------------
void Application::update() {
    timeCurrent = ofGetElapsedTimef();
    timeElapsed = timeCurrent - timeLast;
    timeLast = timeCurrent;

    if (flagHistogramUpdate) {
        refreshHistogram();
    }

    renderer.update(timeElapsed);
}

//--------------------------------------------------------------
void Application::draw() {
    if (currentWindowSize.x > 0 && currentWindowSize.y > 0) {
        prepareGui();

        ofBackground(50);

        renderingFBO.begin();
        {
            ofClear(50);

            renderer.draw(renderingViewport);
        }
        renderingFBO.end();

        renderingFBO.draw(0, 0, currentWindowSize.x, currentWindowSize.y);

        gui.draw();

        if (imageConfig.isCapturing()) {
            imageConfig.saveFrame(renderingFBO);
        }

        if (delaunay.getNumPoints() > 0) {
            ofNoFill();
            delaunay.draw();
        }
    }
}

//--------------------------------------------------------------
void Application::keyPressed(int key) {
    inputController->pressKey(key);
    renderer.onKeyPressed(key);

    if (inputController->isKeyPressed(OF_KEY_CONTROL)) {
        switch (key) {
            case 'q':
                ofExit();
                break;

            default:
                break;
        }
    }
    if (key == OF_KEY_RETURN) {
        elementAttributeWindow.afterReturnKeyReleased();
    }
}

//--------------------------------------------------------------
void Application::keyReleased(int key) {
    inputController->releaseKey(key);
    renderer.onkeyReleased(key);

    if (inputController->isKeyPressed(OF_KEY_CONTROL)) {
        switch (key) {
            case 'n':
                onToolTypeSelected(VectorDrawMode::NONE);
                break;

            case 'p':
                onToolTypeSelected(VectorDrawMode::POINT);
                break;

            case 'l':
                onToolTypeSelected(VectorDrawMode::LINE);
                break;

            case 't':
                onToolTypeSelected(VectorDrawMode::TRIANGLE);
                break;

            case 'r':
                onToolTypeSelected(VectorDrawMode::RECTANGLE);
                break;

            case 'e':
                onToolTypeSelected(VectorDrawMode::ELLIPSE);
                break;

            case 'h':
                onToolTypeSelected(VectorDrawMode::VECTOR_HOUSE);
                break;

            case 's':
                onToolTypeSelected(VectorDrawMode::VECTOR_SNOWMAN);
                break;

            case 'i':
                onToolTypeSelected(VectorDrawMode::IMAGE);
                break;

            default:
                break;
        }
    }
}

//--------------------------------------------------------------
void Application::mouseMoved(int x, int y) {
    renderer.onMouseMoved(x, y);

    ofLogVerbose() << "<app::mouse moved    at: (" << x << ", " << y << ")>";
}

//--------------------------------------------------------------
void Application::mouseDragged(int x, int y, int button) {
    renderer.onMouseMoved(x, y);

    ofLogVerbose() << "<app::mouse dragged  at: (" << x << ", " << y << ") button:" << button << ">";
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button) {
    if (ofxImGui::IsMouseOverGui()) {
        return;
    }

    if (delaunayTriangulation) {
        delaunay.addPoint(ofPoint(x, y));
        delaunay.triangulate();
    }

    inputController->pressMouseButton(button);
    renderer.onMousePressed(x, y, button);

    ofLogVerbose() << "<app::mouse pressed  at: (" << x << ", " << y << ")>";
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button) {
    inputController->releaseMouseButton(button);

    renderer.onMouseReleased(x, y, button);

    ofLogVerbose() << "<app::mouse released at: (" << x << ", " << y << ")>";
}

void Application::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    if (isVerbose) {
        ofLog() << "<app::mouse scrolled at: (" << x << ", " << y << ") for (" << scrollX << ", " << scrollY << ")>";
    }
}

void Application::mouseEntered(int x, int y) {
    renderer.onMouseMoved(x, y);

    ofLogVerbose() << "<app::mouse entered  at: (" << x << ", " << y << ")>";
}

void Application::mouseExited(int x, int y) {
    renderer.onMouseMoved(x, y);

    ofLogVerbose() << "<app::mouse exited   at: (" << x << ", " << y << ")>";
}

void Application::windowResized(int w, int h) {
    currentWindowSize = glm::vec2(w, h);
    if (w > 0 && h > 0) {
        renderingFBO.allocate(w, h, GL_RGBA);
        renderer.onWindowResized(w, h);
    }
}

void Application::exit() {
    ofLog() << "<app::exit>";
}

void Application::prepareGui() {
    auto mainSettings = ofxImGui::Settings();
    mainSettings.windowSize = ofVec2f(200, 200);

    gui.begin();
    bool openExportImageModal = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Import image")) {
                onImportImageButton();
            }
            if (ImGui::MenuItem("Export images")) {
                openExportImageModal = true;
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Quit")) {
                ofExit();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Reset")) {
                onResetButton();
            }
            ImGui::Checkbox("Delaunay", &delaunayTriangulation);

            if (ImGui::MenuItem("Reset Delaunay")) {
                delaunay.reset();
                delaunayTriangulation = false;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {
            ImGui::Checkbox("Scene", &shownWindows[Window::SCENE]);
            ImGui::Checkbox("Properties", &shownWindows[Window::PROPERTIES]);
            ImGui::Checkbox("Materials", &shownWindows[Window::MATERIALS]);
            ImGui::Checkbox("Textures", &shownWindows[Window::TEXTURE]);
            ImGui::Checkbox("Tools", &shownWindows[Window::DRAWING_TOOLS]);
            if (ImGui::Checkbox("Histogram", &shownWindows[Window::HISTOGRAM])) {
                if (shownWindows[Window::HISTOGRAM]) {
                    refreshHistogram();
                }
            }
            ImGui::Checkbox("Camera", &shownWindows[Window::CAMERA]);
            ImGui::Checkbox("Image", &shownWindows[Window::IMAGE]);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            if (ImGui::Checkbox("Verbose logging", &isVerbose)) {
                updateLogLevel();
            }
            if (ImGui::Checkbox("Show origin", &showOrigin)) {
                renderer.toggleOrigin(showOrigin);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (openExportImageModal) {
        ImGui::OpenPopup("Image export");
    }
    imageCaptureModal.prepare();

    ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    dockingFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode;

    ImGuiID dockNodeID = ImGui::DockSpaceOverViewport(NULL, dockingFlags);
    ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(dockNodeID);

    if (dockNode != nullptr) {
        ImGuiDockNode* centralNode = ImGui::DockBuilderGetCentralNode(dockNodeID);
        if (centralNode != nullptr && centralNode->IsEmpty()) {
            ImRect nodeViewport = centralNode->Rect();

            renderingViewport.set(nodeViewport.Min, nodeViewport.Max);
            renderingViewport.translate(-ofGetWindowPositionX(), -ofGetWindowPositionY());
        }
    }

    if (shownWindows[Window::SCENE]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(50 + ofGetWindowPositionX(), 50 + ofGetWindowPositionY());

        sceneGraphWindow.prepare(mainSettings);
    }

    if (shownWindows[Window::PROPERTIES]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(300 + ofGetWindowPositionX(), 50 + ofGetWindowPositionY());

        elementAttributeWindow.prepare(mainSettings);
    }

    if (shownWindows[Window::MATERIALS]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(50 + ofGetWindowPositionX(), 100 + ofGetWindowPositionY());

        materialWindow.prepare(mainSettings);
    }

    if (shownWindows[Window::DRAWING_TOOLS]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(50 + ofGetWindowPositionX(), 300 + ofGetWindowPositionY());

        prepareToolsWindow(mainSettings);
    }

    if (shownWindows[Window::HISTOGRAM]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(300 + ofGetWindowPositionX(), 300 + ofGetWindowPositionY());

        if (ofxImGui::BeginWindow("Histogram", mainSettings)) {
            if (ImGui::Button("Refresh")) {
                onRefreshHistogram();
            }

            auto size = ImGui::GetContentRegionAvail();

            ImGui::Image(GetImTextureID(histogramPixelsID), {size.x, size.y});
        }

        ofxImGui::EndWindow(mainSettings);
    }

    if (shownWindows[Window::CAMERA]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(550 + ofGetWindowPositionX(), 50 + ofGetWindowPositionY());

        cameraWindow.prepare(mainSettings);
    }

    if (shownWindows[Window::TEXTURE]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(550 + ofGetWindowPositionX(), 100 + ofGetWindowPositionY());

        textureWindow.prepare(mainSettings);
    }
    if (shownWindows[Window::IMAGE]) {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        mainSettings.windowPos = ofVec2f(700 + ofGetWindowPositionX(), 50 + ofGetWindowPositionY());

        imageFilter->prepare(mainSettings);
    }

    gui.end();
}

void Application::prepareToolsWindow(ofxImGui::Settings& settings) {
    std::map<VectorDrawMode, std::string> toolsTypeNames = {
        {          VectorDrawMode::NONE,      "None"},
        {         VectorDrawMode::POINT,     "Point"},
        {          VectorDrawMode::LINE,      "Line"},
        {      VectorDrawMode::TRIANGLE,  "Triangle"},
        {     VectorDrawMode::RECTANGLE, "Rectangle"},
        {       VectorDrawMode::ELLIPSE,   "Ellipse"},
        {  VectorDrawMode::VECTOR_HOUSE,     "House"},
        {VectorDrawMode::VECTOR_SNOWMAN,   "Snowman"},
        {         VectorDrawMode::IMAGE,     "Image"}
    };

    ofxImGui::BeginWindow("Drawing Tools", settings);

    const std::string comboPreviewValue = toolsTypeNames[selectedTool];

    if (ImGui::BeginCombo("Tool", comboPreviewValue.c_str(), ImGuiComboFlags_None)) {
        for (const auto& tool : toolsTypeNames) {
            const bool isSelected = tool.first == selectedTool;

            if (ImGui::Selectable(tool.second.c_str(), isSelected)) {
                onToolTypeSelected(tool.first);
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    ImGui::Separator();

    ofxImGui::AddGroup(currentDrawingToolParameters, settings);

    ofxImGui::EndWindow(settings);
}

void Application::onImportImageButton() {
    imageConfig.importImage();
}

void Application::onExportImagesButton(CaptureSettings& settings) {
    imageConfig.startCapture(settings);
}

void Application::onResetButton() {
    renderer.reset();
}

void Application::onToolTypeSelected(VectorDrawMode type) {
    selectedTool = type;
    renderer.setDrawingTool(type);

    currentDrawingToolParameters = renderer.getDrawingTool()->getParameters();
}

void Application::onRefreshHistogram() {
    flagHistogramUpdate = true;
}

void Application::updateLogLevel() {
    ofSetLogLevel(isVerbose ? OF_LOG_VERBOSE : OF_LOG_NOTICE);
}

void Application::refreshHistogram() {
    histogramWrapper.refresh(renderingFBO);

    histogramPixelsID = gui.loadPixels(histogramWrapper.getPixels());

    flagHistogramUpdate = false;
}

}  // namespace engine