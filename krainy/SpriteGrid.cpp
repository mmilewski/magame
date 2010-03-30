#include "SpriteGrid.h"
#include "Engine.h"


SpriteGrid::SpriteGrid() {
    size_t height = Engine::Get().GetRenderer()->GetVerticalTilesOnScreenCount();
    size_t width  = Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount();
    width++; // siatka pamięta o jeden kafel więcej, bo czasami plansza jest ustawiona w połowie kafla
    m_grid.resize(height);
    for (size_t i = 0; i < height; ++i) {
        m_grid.at(i).resize(width); 
    }
}

void SpriteGrid::SetLevel(const LevelPtr lvl, double dx) {
    int half_grid_width = (m_grid.at(0).size()-1) / 2;

    for (size_t y = 0; y < m_grid.size(); ++y) {
        std::vector<SpritePtr>& row = m_grid.at(y);
        for (size_t x = 0; x < row.size(); ++x) {

            int draw_x = x + static_cast<int>(dx) - half_grid_width + 1;
            int draw_y = y;

            const FT::FieldType& ft = lvl->Field(draw_x, draw_y);
            if (ft != FT::None) {
                SetSprite(x, y, m_sprites.at(ft));
            }
            else {
                SetSprite(x, y, SpritePtr());
            }
        }
    }
}

void SpriteGrid::Draw(double dx) const {
    const double tile_width  = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();

    glPushMatrix();
    {
        glTranslatef(dx*tile_width-0.45, 0, 0);

        double offset = dx - static_cast<int>(dx);
        glTranslatef(-offset * tile_width, 0, 0);
        for (size_t y = 0; y < m_grid.size(); ++y) {
            const std::vector<SpritePtr>& row = m_grid.at(y);
            for (size_t x = 0; x < row.size(); ++x) {
                const SpritePtr& sprite = row.at(x);
                if (sprite) {
                    sprite->DrawCurrentFrame(x * tile_width, 1.0 - (y+1) * tile_height, 
                                             tile_width, tile_height);
                }
            }
        }
    }
    glPopMatrix();
}


void SpriteGrid::StoreSprite(FT::FieldType ft, SpritePtr sp) {
    if (m_sprites.size() <= static_cast<size_t>(ft)) m_sprites.resize(ft + 1);
    m_sprites.at(ft) = sp;
}
