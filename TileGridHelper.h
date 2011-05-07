#ifndef TILEGRIDHELPER_H
#define	TILEGRIDHELPER_H

#include "StdAfx.h"
#include "BasicMathTypes.h"

class TileGridHelper {
public:
    explicit TileGridHelper(const Position& beg, const Position& end);

    // Przyciąga podany punkt do najbliższego punktu siatki
    TileGridHelper& SnapToGrid();

    // Zmienia współrzędne podanych punktów tak, ze w beg znajdują się
    // minimalne (lewy dolny narożnik), a w end maksymalne (prawy górny narożnik).
    TileGridHelper& SortCoordsOfBox();

    // Wymagają posortowanych współrzędnych
    unsigned TilesHorizontally() const;
    unsigned TilesVertically() const;

    Position Beg() const { return m_beg; }
    Position End() const { return m_end; }

private:
    Position m_beg, m_end;
};

#endif
