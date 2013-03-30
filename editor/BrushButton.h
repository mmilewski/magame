#ifndef MAGAME_BRUSHBUTTON_H__INCLUDED
#define MAGAME_BRUSHBUTTON_H__INCLUDED
#include "StdAfx.h"

#include "gui/SpriteButton.h"
#include "Brush.h"

class BrushButton;
typedef boost::shared_ptr<BrushButton> BrushButtonPtr;

class BrushButton : public gui::SpriteButton {
public:
    explicit BrushButton(SpritePtr sprite, Position position, Size size, BrushPtr brush)
        : SpriteButton(sprite, position, size),
          m_brush(brush)  {
    }
    BrushPtr  GetBrush()  const    { return m_brush; }

private:
    BrushPtr m_brush;
};

#endif
