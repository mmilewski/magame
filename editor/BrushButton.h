#ifndef __BRUSHBUTTON_H__INCLUDED__
#define __BRUSHBUTTON_H__INCLUDED__

#include "../gui/SpriteButton.h"
#include "Brush.h"

class BrushButton;
typedef boost::shared_ptr<BrushButton> BrushButtonPtr;

class BrushButton : public SpriteButton {
public:
    explicit BrushButton(SpritePtr sprite, Position position, Size size, BrushPtr brush)
        : SpriteButton(sprite, position, size),
          m_brush(brush)  {
    }
    BrushPtr  GetBrush()  const    { return m_brush; }
private:
    BrushPtr m_brush;
};


class MultiBrushButton;
typedef boost::shared_ptr<MultiBrushButton> MultiBrushButtonPtr;

class MultiBrushButton : public SpriteButton {
public:
    explicit MultiBrushButton(SpritePtr sprite, Position position, Size size, MultiBrushPtr MultiBrush)
        : SpriteButton(sprite, position, size),
          m_MultiBrush(MultiBrush)  {
    }
    MultiBrushPtr  GetMultiBrush()  const    { return m_MultiBrush; }
private:
    MultiBrushPtr m_MultiBrush;
};
#endif
