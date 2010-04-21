#include "StdAfx.h"

#include "Engine.h"
#include "Sprite.h"

Sprite::Sprite(const SpriteConfigData& data) :
    m_data(data), m_current_frame(0), m_current_frame_duration(0.0) {

}

void Sprite::SetCurrentFrame(size_t frame_num) {
    m_current_frame = frame_num;
    m_current_frame_duration = 0.0;

}

void Sprite::Update(double dt) {
    m_current_frame_duration += dt;

    // przejdź do następnej klatki
    if (m_current_frame_duration >= m_data.frame_duration_time) {
        m_current_frame++;
        m_current_frame_duration -= m_data.frame_duration_time;
    }
    // sprawdź czy nastąpił koniec animacji - przejdź do klatki 0. lub ostatniej
    if (m_current_frame >= m_data.frame_count) {
        if (m_data.loop) {
            m_current_frame = 0;
        } else {
            m_current_frame = m_data.frame_count - 1;
        }
    }
}

void Sprite::DrawCurrentFrame(double x, double y, double width, double height) {
    Engine::Get().GetRenderer()->DrawSprite(
            m_data.left + m_data.width * m_current_frame, m_data.bottom,
            m_data.width, m_data.height, x, y, width, height, m_data.layer);
}

