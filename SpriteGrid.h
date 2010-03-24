#ifndef __SPRITE_GRID_H__
#define __SPRITE_GRID_H__
#include "StdAfx.h"

#include <vector> 
#include "Sprite.h"
#include "Level.h"

class SpriteGrid {
public:
    explicit SpriteGrid();

    void SetLevel(const LevelPtr lvl, double dx);
    void Draw(double dx) const;

    void StoreSprite(FT::FieldType ft, SpritePtr p);

private:
    void SetSprite(size_t x, size_t y, SpritePtr sprite) { m_grid.at(y).at(x) = sprite; }

private:
    std::vector< std::vector< SpritePtr > > m_grid;
    std::vector<SpritePtr> m_sprites;
};


#endif
