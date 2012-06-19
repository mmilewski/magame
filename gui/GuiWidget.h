#ifndef MAGAME_GUIWIDGET_H_INCLUDED
#define MAGAME_GUIWIDGET_H_INCLUDED
#include "StdAfx.h"

#include "math/BasicMathTypes.h"
#include "math/Aabb.h"

class GuiWidget;
typedef boost::shared_ptr<GuiWidget> GuiWidgetPtr;

class GuiWidget {
public:
    explicit GuiWidget(Position position, Size size, bool is_visible)
        : m_position(position),
          m_size(size),
          m_is_visible(is_visible) {
    }
    virtual ~GuiWidget() {}

    virtual void Draw() const = 0;
    virtual void Update(double dt) = 0;

    Position GetPosition()      const { return m_position; }
    Position GetSizedPosition() const { return Position(GetPosition().X() + GetSize().X(),
                                                        GetPosition().Y() + GetSize().Y()); }
    Size     GetSize()          const { return m_size; }
    bool     IsVisible()        const { return m_is_visible; }
    GuiWidget* MoveBy(const Vector2& vector)       { m_position += vector; return this; }
    GuiWidget* SetSize(const Vector2& vector)      { m_size = vector;      return this; }
    GuiWidget* SetPosition(const Vector2& vector)  { m_position = vector;  return this; }
    GuiWidget* SetVisible(bool is_visible)         { m_is_visible = is_visible; return this; }

    Aabb GetAabb() const {
        return Aabb(GetPosition().X(), GetPosition().Y(),
                    GetSizedPosition().X(), GetSizedPosition().Y());
    }
private:
    Position m_position;             // położenia (lewego dolnego narożnika) na ekranie (przestrzeń okna)
    Size m_size;                     // rozmiar (szerokość i wysokość) (przestrzeń okna)
    bool m_is_visible;               // czy kontrolka jest widoczna
};

#endif
