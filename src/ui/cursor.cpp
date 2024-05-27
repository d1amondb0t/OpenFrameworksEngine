#include "cursor.h"

#include "ofMain.h"
void Cursor::setup() {
    type = CursorType::CROSS;

    mouseCurrentX = mouseCurrentY = 0;

    isMouseButtonPressed = false;
}

void Cursor::draw() {
    ofSetLineWidth(2);

    if (isMouseButtonPressed) {
        ofSetColor(255, 0, 0);
    } else {
        ofSetColor(0);
    }

    switch (type) {
        case CursorType::CROSS: {
            drawCross(mouseCurrentX, mouseCurrentY);
            break;
        }
        case CursorType::CIRCLE: {
            drawCircle(mouseCurrentX, mouseCurrentY);
            break;
        }
        case CursorType::UP: {
            drawUp(mouseCurrentX, mouseCurrentY);
            break;
        }
        case CursorType::DOWN: {
            drawDown(mouseCurrentX, mouseCurrentY);
            break;
        }
        case CursorType::LEFT: {
            drawLeft(mouseCurrentX, mouseCurrentY);
            break;
        }
        case CursorType::RIGHT: {
            drawRight(mouseCurrentX, mouseCurrentY);
            break;
        }
    }
}

void Cursor::setType(CursorType type) {
    this->type = type;
}

void Cursor::onMouseMoved(int x, int y) {
    mouseCurrentX = x;
    mouseCurrentY = y;
}

void Cursor::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseButtonPressed = true;

    }
    else if (button == OF_MOUSE_BUTTON_RIGHT) {
        setType(CursorType::CIRCLE);
    }

    mouseCurrentX = x;
    mouseCurrentY = y;
}

void Cursor::onMouseReleased(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseButtonPressed = false;
    } 
    else if (button == OF_MOUSE_BUTTON_RIGHT) {
        setType(CursorType::CROSS);
    }

    mouseCurrentX = x;
    mouseCurrentY = y;
}


void Cursor::onKeyPressed(int key) {
    if (key == 'w') {
        setType(CursorType::UP);
    }
    if (key == 's') {
        setType(CursorType::DOWN);
    }
    if (key == 'a') {
        setType(CursorType::LEFT);
    }
    if (key == 'd') {
        setType(CursorType::RIGHT);
    }
}

void Cursor::onKeyReleased(int key) {
    setType(CursorType::CROSS);
}


float Cursor::getMouseCurrentX() {
    return mouseCurrentX;
}

float Cursor::getMouseCurrentY() {
    return mouseCurrentY;
}

void Cursor::drawCross(float x, float y) const {
    ofDrawLine(x - cursorLength, y, x + cursorLength, y);
    ofDrawLine(x, y - cursorLength, x, y + cursorLength);

    ofDrawLine(x - cursorLength, y, x - cursorLength + cursorSmallLength, y - cursorSmallLength);
    ofDrawLine(x - cursorLength, y, x - cursorLength + cursorSmallLength, y + cursorSmallLength);

    ofDrawLine(x + cursorLength, y, x + cursorLength - cursorSmallLength, y + cursorSmallLength);
    ofDrawLine(x + cursorLength, y, x + cursorLength - cursorSmallLength, y - cursorSmallLength);

    ofDrawLine(x, y - cursorLength, x + cursorSmallLength, y - cursorLength + cursorSmallLength);
    ofDrawLine(x, y - cursorLength, x - cursorSmallLength, y - cursorLength + cursorSmallLength);

    ofDrawLine(x, y + cursorLength, x + cursorSmallLength, y + cursorLength - cursorSmallLength);
    ofDrawLine(x, y + cursorLength, x - cursorSmallLength, y + cursorLength - cursorSmallLength);
}

void Cursor::drawCircle(float x, float y) const {
    ofNoFill();
    ofDrawCircle(x, y, 5.f);
}

void Cursor::drawUp(float x, float y) const {
    ofDrawLine(x, y - cursorLength, x, y);
    ofDrawLine(x, y - cursorLength, x + cursorSmallLength, y - cursorLength + cursorSmallLength);
    ofDrawLine(x, y - cursorLength, x - cursorSmallLength, y - cursorLength + cursorSmallLength);
}

void Cursor::drawDown(float x, float y) const {
    ofDrawLine(x, y, x, y + cursorLength);
    ofDrawLine(x, y + cursorLength, x + cursorSmallLength, y + cursorLength - cursorSmallLength);
    ofDrawLine(x, y + cursorLength, x - cursorSmallLength, y + cursorLength - cursorSmallLength);
}

void Cursor::drawLeft(float x, float y) const {
    ofDrawLine(x - cursorLength, y, x, y);
    ofDrawLine(x - cursorLength, y, x - cursorLength + cursorSmallLength, y - cursorSmallLength);
    ofDrawLine(x - cursorLength, y, x - cursorLength + cursorSmallLength, y + cursorSmallLength);
}

void Cursor::drawRight(float x, float y) const {
    ofDrawLine(x, y, x + cursorLength, y);
    ofDrawLine(x + cursorLength, y, x + cursorLength - cursorSmallLength, y + cursorSmallLength);
    ofDrawLine(x + cursorLength, y, x + cursorLength - cursorSmallLength, y - cursorSmallLength);
}
