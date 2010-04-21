#ifndef __SPRITE_POOL_H__
#define __SPRITE_POOL_H__
#include "StdAfx.h"

#include "Types.h"
#include "Sprite.h"

class SpritePool {
public:
    SpritePtr SpriteByFieldType(FT::FieldType type) const { return m_sprites.at(type); }
    void SetSprite(FT::FieldType type, SpritePtr sprite);
    void Update(double dt);

private:
    std::vector<SpritePtr> m_sprites;
};

typedef boost::shared_ptr<SpritePool> SpritePoolPtr;

#endif
