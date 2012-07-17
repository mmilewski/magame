#ifndef MAGAME_SPRITEBUTTON_H__INCLUDED
#define MAGAME_SPRITEBUTTON_H__INCLUDED
#include "StdAfx.h"

#include "Widget.h"
#include "video/Sprite.h"
#include "video/Renderer.h"

namespace gui {

class SpriteButton;
typedef boost::shared_ptr<SpriteButton> SpriteButtonPtr;

class SpriteButton : public Widget {
public:
    explicit SpriteButton(SpritePtr sprite, Position position, Size size)
        : Widget(position, size, true),
          m_sprite(sprite) {
    }
    virtual ~SpriteButton()   {}

    virtual void Draw() const      { m_sprite->DrawCurrentFrame(GetPosition(), GetSize()); }
    virtual void Update(double dt) { m_sprite->Update(dt); }

    SpritePtr GetSprite() const    { return m_sprite; }

private:
    SpritePtr m_sprite;
};


} // namespace gui

#endif  // MAGAME_SPRITEBUTTON_H__INCLUDED
