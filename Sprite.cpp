#include "StdAfx.h"

#include "Engine.h"
#include "Sprite.h"

Sprite::Sprite(const SpriteConfigData& data) :
    m_data(data), m_current_frame(0), m_current_frame_duration(0.0),
    m_width_repeat(-1), m_height_repeat(-1),
    m_dark(data.dark) {

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

void Sprite::DrawCurrentFrame(Position position, Size size) const {
    DrawCurrentFrame(position.X(), position.Y(), size.X(), size.Y());
}

void Sprite::DrawCurrentFrame(double x, double y, double width, double height) const {
    // jeżeli powtarzanie jest nieaktywne, to rysujemy normalnie
    if (m_width_repeat < 0 && m_height_repeat < 0) {
        if (m_dark) {
            Engine::Get().GetRenderer()->DrawSprite(
                m_data.left + m_data.width * m_current_frame, m_data.bottom,
                m_data.width, m_data.height, x, y, width, height, m_data.layer,
                0.15);
        }
        else {
            Engine::Get().GetRenderer()->DrawSprite(
                m_data.left + m_data.width * m_current_frame, m_data.bottom,
                m_data.width, m_data.height, x, y, width, height, m_data.layer);
        }
        return;
    }

    //
    // poniżej znajduje się kod dla rysowania z włączonym powtarzaniem
    //

    const double rep_width = m_width_repeat;                           // rozmiary powtarzanego sprite'a
    const double rep_height = m_height_repeat;
    const int whole_in_x = static_cast<int>(width / rep_width);        // liczba całych sprite'ów na każdej z osi
    const int whole_in_y = static_cast<int>(height / rep_height);
    const double scrap_width = width - whole_in_x * rep_width;         // rozmiar skrawków (części brakujących do pełnych klocków)
    const double scrap_height = height - whole_in_y * rep_height;
    const double tex_x = m_data.left + m_data.width * m_current_frame; // położenie sprite'a w teksturze (zawsze takie samo)
    const double tex_y = m_data.bottom;

    // rysuj całe sprite'y
    for (int ix=0; ix < whole_in_x; ++ix) {
        for (int iy=0; iy < whole_in_y; ++iy) {
            Engine::Get().GetRenderer()->DrawSprite(
                tex_x,  tex_y,                                                 // tex pos
                m_data.width,  m_data.height,                                  // tex size
                x + rep_width * ix,  y + rep_height * iy,                      // screen pos
                rep_width,  rep_height,                                        // screen size
                m_data.layer);
        }
    }

    // dla każdego y'ka narysuj skrawki x'ów
    for (int iy=0; iy < whole_in_y && scrap_width > 0; ++iy) {
        Engine::Get().GetRenderer()->DrawSprite(
            tex_x,  tex_y,                                                     // tex pos
            m_data.width * (scrap_width/rep_width),  m_data.height,            // tex size
            x + rep_width * whole_in_x,   y + rep_height * iy,                 // screen pos
            scrap_width,  std::max(scrap_height,  rep_height),                 // screen size
            m_data.layer);
    }

    // dla każdego x'a narysuj skrawki y'ów
    for (int ix=0; ix < whole_in_x && scrap_height > 0; ++ix) {
        Engine::Get().GetRenderer()->DrawSprite(
            tex_x,  tex_y,                                                     // tex pos
            m_data.width,  m_data.height * (scrap_height/rep_height),          // tex size
            x + rep_width * ix,  y + rep_height * whole_in_y,                  // screen pos
            std::max(scrap_width,  rep_width),  scrap_height,                  // screen size
            m_data.layer);
    }

    // skrawek z x'a i y'ka -- narożnik
    if (scrap_height > 0 && scrap_width > 0) {
        Engine::Get().GetRenderer()->DrawSprite(
            tex_x,  tex_y,                                                                       // tex pos
            m_data.width * (scrap_width/rep_width),  m_data.height * (scrap_height/rep_height),  // tex size
            x + rep_width * whole_in_x,  y + rep_height * whole_in_y,                            // screen pos
            scrap_width,  scrap_height,                                                          // screen size
            m_data.layer);
    }
}

void Sprite::SetRepeat(double width, double height) {
    m_width_repeat = width;
    m_height_repeat = height;
}

void Sprite::ResetRepeat() {
    m_height_repeat = m_width_repeat = -1;
}

SpritePtr Sprite::GetByName(const std::string& name) {
    if (name == "multi_platform") {
        return SpritePtr(new Sprite(Engine::Get().GetSpriteConfig()->Get("PlatformTop")));
    }
    return SpritePtr(new Sprite(Engine::Get().GetSpriteConfig()->Get(name)));
}
