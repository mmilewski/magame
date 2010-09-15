#ifndef __SPRITE_GRID_H__
#define __SPRITE_GRID_H__
#include "StdAfx.h"

#include "Sprite.h"
#include "Level.h"


class SpriteGrid {
public:
    explicit SpriteGrid();

    void SetLevel(const LevelPtr lvl, double dx);
    void Draw(double dx) const;
    void DrawHighlighted(double dx, int x, int y) const;

    void StoreSprite(FT::FieldType ft, SpritePtr p);

    // Zwraca pozycję (współrzędne okna), na której znajduje się
    // pole w wskazanym położeniu (współrzędne świata)
    Position GetWindowPositionOfField(int x, int y) const;
    Position GetWindowPositionOfField(Position p) const { return GetWindowPositionOfField(p.X(), p.Y()); }

    // Zwraca rozmiar (współrzędne okna) pola
    // o wskazanym położeniu (współrzędne świata)
    Size     GetWindowSizeOfField(int x, int y) const;
    Size     GetWindowSizeOfField(Position p) const { return GetWindowSizeOfField(p.X(), p.Y()); }

private:
    void SetSprite(size_t x, size_t y, SpritePtr sprite) { m_grid.at(y).at(x) = sprite; }

private:
    std::vector< std::vector< SpritePtr > > m_grid;
    std::vector<SpritePtr> m_sprites;
};


#endif
