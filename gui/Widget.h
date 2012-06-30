#ifndef MAGAME_WIDGET_H_INCLUDED
#define MAGAME_WIDGET_H_INCLUDED
#include "StdAfx.h"

#include "math/BasicMathTypes.h"
#include "math/Aabb.h"

namespace gui {

class Widget;
typedef boost::shared_ptr<Widget> WidgetPtr;

class Widget {
public:
    explicit Widget(Position position, Size size, bool is_visible)
        : m_position(position),
          m_size(size),
          m_is_visible(is_visible) {
    }
    virtual ~Widget() {}

    virtual void Draw() const = 0;
    virtual void Update(double dt) = 0;

    Position GetPosition()      const { return m_position; }
    Position GetSizedPosition() const { return GetPosition() + GetSize(); }
    Size     GetSize()          const { return m_size; }

    bool     IsVisible()        const { return m_is_visible; }
    Widget* MoveBy(const Vector2& vector)       { m_position += vector; return this; }
    Widget* SetSize(const Vector2& vector)      { m_size = vector;      return this; }
    Widget* SetPosition(const Vector2& vector)  { m_position = vector;  return this; }
    Widget* SetVisible(bool is_visible)         { m_is_visible = is_visible; return this; }

    Aabb GetAabb() const {
        return Aabb(GetPosition().X(), GetPosition().Y(),
                    GetSizedPosition().X(), GetSizedPosition().Y());
    }
private:
    Position m_position;             // położenia (lewego dolnego narożnika) na ekranie (przestrzeń okna)
    Size m_size;                     // rozmiar (szerokość i wysokość) (przestrzeń okna)
    bool m_is_visible;               // czy kontrolka jest widoczna
};

} // namespace gui

#endif
