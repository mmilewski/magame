#include "../StdAfx.h"

#include "../Engine.h"
#include "EditorGui.h"

EditorGui::EditorGui() {
}

void EditorGui::Start() {
}

void EditorGui::Init() {
    m_buttons.clear();
    const Size default_size = Size(.1, .1);
#define BUTTON(name,pos,size,type) BrushButtonPtr(new BrushButton(Sprite::GetByName(name), pos, size, Brush::New(Sprite::GetByName(name), type)))
#define ADD_BUTTON(name,pos,size,type) m_buttons.push_back(BUTTON(name,pos,size,type))
#define MULTIBUTTON(name,pos,size,type) BrushButtonPtr(new BrushButton(Sprite::GetByName(name), pos, size, MultiBrush::New(Sprite::GetByName(name))))
#define ADD_MULTIBUTTON(name,pos,size,type) m_buttons.push_back(MULTIBUTTON(name,pos,size,type))

    // nazwy sprite'ów zdefiniowane są w SpriteConfig::SpriteConfig
    ADD_BUTTON("gui_eraser",           Position( 0, .0), default_size*2, Brush::ST::Eraser);
    ADD_MULTIBUTTON("PlatformMid",     Position(.8, .1), default_size, Brush::ST::Multi);

    ADD_BUTTON("player_stop",          Position(.1, .8), default_size, Brush::ST::Player);
    ADD_BUTTON("EndOfLevel",           Position(.2, .8), default_size, FT::EndOfLevel);
    ADD_BUTTON("twinshot_upgrade",     Position(.4, .8), default_size, ET::TwinShot);
    ADD_BUTTON("mush_stop",            Position(.8, .8), default_size, ET::Mush);

    ADD_BUTTON("PlatformTopLeft",      Position(.3, .5),  default_size, FT::PlatformTopLeft);
    ADD_BUTTON("PlatformTop",          Position(.4, .5),  default_size, FT::PlatformTop);
    ADD_BUTTON("PlatformTopRight",     Position(.5, .5),  default_size, FT::PlatformTopRight);
    ADD_BUTTON("PlatformLeft",         Position(.3, .4),  default_size, FT::PlatformLeft);
    ADD_BUTTON("PlatformMid",          Position(.4, .4),  default_size, FT::PlatformMid);
    ADD_BUTTON("PlatformRight",        Position(.5, .4),  default_size, FT::PlatformRight);
    ADD_BUTTON("PlatformLeftRight",    Position(.4, .15), default_size, FT::PlatformLeftRight);
    ADD_BUTTON("PlatformLeftTopRight", Position(.4, .25), default_size, FT::PlatformLeftTopRight);

#undef ADD_MULTIBUTTON
#undef MULTIBUTTON
#undef ADD_BUTTON
#undef BUTTON
}

void EditorGui::Draw() {
    // ciemne tło
    Engine::Get().GetRenderer()->DrawQuad(0, 0, 1, 1, Rgba(0,0,0,.7));

    // widoczne kontrolki
    BOOST_FOREACH(const auto& button, m_buttons) {
        if (button->IsVisible() && button!=m_hovered_button) {
            button->Draw();
        }
    }

    // "najechany" przycisk rysujemy osobno.
    if (m_hovered_button) {
        m_hovered_button->Draw();
        const Aabb& box = m_hovered_button->GetAabb();
        Engine::Get().GetRenderer()->DrawQuad(box.GetMinX(), box.GetMinY(),
                                              box.GetMaxX(), box.GetMaxY(),
                                              Rgba(1,1,1, .4));
    }
}

void EditorGui::Update(double dt) {
    boost::for_each(m_buttons, boost::bind(&gui::Widget::Update, _1, dt));
}

bool EditorGui::OnMouseMove(double x, double y) {
    const Aabb cursor_aabb = Aabb(x, y, x+.02, y+0.02);
    m_hovered_button.reset();
    BOOST_FOREACH(const auto& button, m_buttons) {
        if (button->IsVisible() && button->GetAabb().Collides(cursor_aabb)) {
            m_hovered_button = button;
        }
    }
    return bool(m_hovered_button);
}

bool EditorGui::OnMouseDown(Uint8 /* button */, double /* x */, double /* y */) {
    if (!m_hovered_button) {
        return false;
    }
    m_active_brush = m_hovered_button->GetBrush();
    return true;
}
