#include "StdAfx.h"

#include "Sprite.h"
#include "SavePoint.h"

SavePoint::SavePoint(double x, double y)
  : Entity(x, y, 0, 0),
    m_is_active(false) {

}

void SavePoint::Update(double dt, LevelPtr level) {
    m_sprite_active->Update(dt);
    m_sprite_inactive->Update(dt);
}

void SavePoint::Draw() const {
    const double tile_width = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();

    // wylicz pozycję gracza na ekranie
    const double pos_x = m_x * tile_width;
    const double pos_y = m_y * tile_height;

    if (IsActive()) {
        m_sprite_active->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
    } else {
        m_sprite_inactive->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
    }
    
}

void SavePoint::SetSprites(SpritePtr active, SpritePtr inactive) {
    m_sprite_active = active;
    m_sprite_inactive = inactive;
}

void SavePoint::Activate() {
    m_is_active = true;
}