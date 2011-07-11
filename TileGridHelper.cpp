#include "TileGridHelper.h"

TileGridHelper::TileGridHelper(const Position& beg, const Position& end)
    : m_beg(beg), m_end(end) {
}

TileGridHelper& TileGridHelper::SnapToGrid() {
    m_beg = Position(round(m_beg.X()), round(m_beg.Y()));
    m_end = Position(round(m_end.X()), round(m_end.Y()));
    return *this;
}

unsigned TileGridHelper::TilesHorizontally() const {
    return static_cast<unsigned>(round(m_end[0] - m_beg[0]));
}

unsigned TileGridHelper::TilesVertically() const {
    return static_cast<unsigned>(round(m_end[1] - m_beg[1]));
}

TileGridHelper& TileGridHelper::SortCoordsOfBox() {
    Position orga = m_beg, orgb = m_end;
    m_beg[0] = std::min(orga.X(), orgb.X());
    m_end[0] = std::max(orga.X(), orgb.X());
    m_beg[1] = std::min(orga.Y(), orgb.Y());
    m_end[1] = std::max(orga.Y(), orgb.Y());
    return *this;
}

