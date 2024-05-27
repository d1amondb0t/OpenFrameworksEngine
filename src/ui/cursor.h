#pragma once

enum CursorType {CROSS, CIRCLE, UP, DOWN, LEFT, RIGHT };

class Cursor {
  public:
    void setup();
    void draw();
    void setType(CursorType type);

    void onMouseMoved(int x, int y);
    void onMousePressed(int x, int y, int button);
    void onMouseReleased(int x, int y, int button);

    void onKeyPressed(int key);
    void onKeyReleased(int key);

    float getMouseCurrentX();
    float getMouseCurrentY();

  private:
    CursorType type;

    float cursorLength = 10.0f;
    float cursorSmallLength = 3.f;

    int mouseCurrentX;
    int mouseCurrentY;

    bool isMouseButtonPressed;


    void drawCross(float x, float y) const;
    void drawCircle(float x, float y) const;
    void drawUp(float x, float y) const;
    void drawDown(float x, float y) const;
    void drawLeft(float x, float y) const;
    void drawRight(float x, float y) const;
};
