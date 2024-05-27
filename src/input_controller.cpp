#include "input_controller.h"

engine::InputController::InputController() {
    // OF_MOUSE_BUTTON_[1..3] have the same values as Left, Middle and Right
    mouseButtonsPressed = {
        {  OF_MOUSE_BUTTON_LEFT, false},
        {OF_MOUSE_BUTTON_MIDDLE, false},
        { OF_MOUSE_BUTTON_RIGHT, false},
        {     OF_MOUSE_BUTTON_4, false},
        {     OF_MOUSE_BUTTON_5, false},
        {     OF_MOUSE_BUTTON_6, false},
        {     OF_MOUSE_BUTTON_7, false},
        {     OF_MOUSE_BUTTON_8, false},
    };

    // OF_KEY_COMMAND and OF_KEY_SUPER share the same value
    keysPressed = {
        {       OF_KEY_RETURN, false},
        {          OF_KEY_ESC, false},
        {          OF_KEY_TAB, false},

        {    OF_KEY_BACKSPACE, false},
        {          OF_KEY_DEL, false},

        {        OF_KEY_SHIFT, false},
        {      OF_KEY_CONTROL, false},
        {          OF_KEY_ALT, false},
        {        OF_KEY_SUPER, false},

        {   OF_KEY_LEFT_SHIFT, false},
        {  OF_KEY_RIGHT_SHIFT, false},
        { OF_KEY_LEFT_CONTROL, false},
        {OF_KEY_RIGHT_CONTROL, false},
        {     OF_KEY_LEFT_ALT, false},
        {    OF_KEY_RIGHT_ALT, false},
        {   OF_KEY_LEFT_SUPER, false},
        {  OF_KEY_RIGHT_SUPER, false},

        {           OF_KEY_F1, false},
        {           OF_KEY_F2, false},
        {           OF_KEY_F3, false},
        {           OF_KEY_F4, false},
        {           OF_KEY_F5, false},
        {           OF_KEY_F6, false},
        {           OF_KEY_F7, false},
        {           OF_KEY_F8, false},
        {           OF_KEY_F9, false},
        {          OF_KEY_F10, false},
        {          OF_KEY_F11, false},
        {          OF_KEY_F12, false},
        {         OF_KEY_LEFT, false},
        {           OF_KEY_UP, false},
        {        OF_KEY_RIGHT, false},
        {         OF_KEY_DOWN, false},
        {      OF_KEY_PAGE_UP, false},
        {    OF_KEY_PAGE_DOWN, false},
        {         OF_KEY_HOME, false},
        {          OF_KEY_END, false},
        {       OF_KEY_INSERT, false},
    };

    for (int i = 'A'; i <= 'z'; i++) {
        if (!keyExists(i)) {
            keysPressed[i] = false;
        }
    }
}

bool engine::InputController::isKeyPressed(int key) {
    return keyExists(key) && keysPressed[key];
}

void engine::InputController::pressKey(int key) {
    if (keyExists(key)) {
        keysPressed[key] = true;
    }
}

void engine::InputController::releaseKey(int key) {
    if (keyExists(key)) {
        keysPressed[key] = false;
    }
}

bool engine::InputController::isMouseButtonPressed(int button) {
    return mouseButtonExists(button) && mouseButtonsPressed[button];
}

void engine::InputController::pressMouseButton(int button) {
    if (mouseButtonExists(button)) {
        mouseButtonsPressed[button] = true;
    }
}

void engine::InputController::releaseMouseButton(int button) {
    if (mouseButtonExists(button)) {
        mouseButtonsPressed[button] = false;
    }
}

bool engine::InputController::keyExists(int key) {
    return keysPressed.find(key) != keysPressed.end();
}

bool engine::InputController::mouseButtonExists(int button) {
    return mouseButtonsPressed.find(button) != mouseButtonsPressed.end();
}
