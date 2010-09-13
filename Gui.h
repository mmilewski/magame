#ifndef __GUI_H_INCLUDED__
#define __GUI_H_INCLUDED__
#include "StdAfx.h"


class Gui;
typedef boost::shared_ptr<Gui> GuiPtr;

class Gui : public boost::enable_shared_from_this<Gui> {
public:
    explicit Gui() {
    }

    virtual ~Gui() {}

    virtual void Start() = 0;
    virtual void Init() = 0;
    virtual void Draw() = 0;
    virtual void Update(double dt) = 0;

    virtual bool OnKeyDown(const SDLKey& key) { return false; }
    virtual bool OnKeyUp(const SDLKey& key)   { return false; }
    virtual bool OnMouseMove(double x, double y) { return false; }
    virtual bool OnMouseDown(Uint8 button, double x, double y) { return false; }
};

#include "BasicMathTypes.h"

class GuiWidget {
public:
    explicit GuiWidget(Position position, Size size, bool is_visible)
        : m_position(position),
          m_size(size),
          m_is_visible(is_visible) {
    }
    virtual ~GuiWidget() {}

    virtual void Draw() = 0;
    virtual void Update(double dt) = 0;

    Position GetPosition() const { return m_position; }
    Size     GetSize() const     { return m_size; }
    bool     IsVisible() const   { return m_is_visible; }
    GuiWidget* MoveBy(const Vector2& vector)       { m_position += vector; return this; }
    GuiWidget* SetSize(const Vector2& vector)      { m_size = vector;      return this; }
    GuiWidget* SetPosition(const Vector2& vector)  { m_position = vector;  return this; }
    GuiWidget* SetVisible(bool is_visible)         { m_is_visible = is_visible; return this; }

private:
    Position m_position;             // położenia na ekranie (przestrzeń okna)
    Size m_size;                     // rozmiar (szerokość i wysokość) (przestrzeń okna)
    bool m_is_visible;               // czy kontrolka jest widoczna
};

class SpriteButton : public GuiWidget {
public:
    explicit SpriteButton(SpritePtr sprite, Position position, Size size)
        : GuiWidget(position, size, true),
          m_sprite(sprite) {
        // nop
    }
    virtual void Draw() const      { m_sprite->DrawCurrentFrame(GetPosition(), GetSize()); }
    virtual void Update(double dt) { m_sprite->Update(dt); }
private:
    SpritePtr m_sprite;
};

class EditorGui : public Gui, public boost::enable_shared_from_this<EditorGui> {
public:
    void Start() {}
    void Init() {}
    void Draw() {}
    void Update(double dt) {}
};



#endif
