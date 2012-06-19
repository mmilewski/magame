#ifndef MAGAME_SPRITEBUTTON_H__INCLUDED
#define MAGAME_SPRITEBUTTON_H__INCLUDED
#include "StdAfx.h"

#include "GuiWidget.h"
#include "video/Sprite.h"
#include "video/Renderer.h"

class SpriteButton;
typedef boost::shared_ptr<SpriteButton> SpriteButtonPtr;

class SpriteButton : public GuiWidget {
public:
    explicit SpriteButton(SpritePtr sprite, Position position, Size size)
        : GuiWidget(position, size, true),
          m_sprite(sprite) {
    }
    virtual ~SpriteButton()   {}

    virtual void Draw() const      { m_sprite->DrawCurrentFrame(GetPosition(), GetSize()); DoDraw(); }
    virtual void Update(double dt) { m_sprite->Update(dt); DoUpdate(); }

    SpritePtr GetSprite() const    { return m_sprite; }

protected:
    virtual void DoDraw() const {}
    virtual void DoUpdate() {}

private:
    SpritePtr m_sprite;
};

#endif  // MAGAME_SPRITEBUTTON_H__INCLUDED
