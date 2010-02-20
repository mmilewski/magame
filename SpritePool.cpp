#include "SpritePool.h"

void SpritePool::SetSprite(FT::FieldType type, SpritePtr sprite) {
    if (m_sprites.size() <= static_cast<size_t>(type)) m_sprites.resize(type + 1);
    m_sprites.at(type) = sprite;
}

void SpritePool::Update(double dt) {
    for (size_t i = 0; i < m_sprites.size(); ++i) {
        if (m_sprites.at(i)) {
            m_sprites.at(i)->Update(dt);
        }
    }
}
