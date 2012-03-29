#ifndef MAGAME_SPRITE_GRID_H_INCLUDED
#define MAGAME_SPRITE_GRID_H_INCLUDED
#include "StdAfx.h"

#include "Sprite.h"
#include "Level.h"


class SpriteGrid {
public:
    explicit SpriteGrid();

    void SetLevel(const LevelPtr lvl, double dx);
    void Draw(double dx) const;
    void Update(double dt);

    void StoreSprite(FT::FieldType ft, SpritePtr p);

private:
    void SetSprite(size_t x, size_t y, SpritePtr sprite) { m_grid.at(y).at(x) = sprite; }

private:
    typedef std::vector<SpritePtr> GridRow;
    std::vector<GridRow> m_grid;
    std::map<FT::FieldType, SpritePtr> m_sprites;
};


#endif /* MAGAME_SPRITE_GRID_H_INCLUDED */
