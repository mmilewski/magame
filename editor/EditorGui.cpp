#include "../StdAfx.h"

#include "../Engine.h"
#include "EditorGui.h"

EditorGui::EditorGui() {
}

void EditorGui::Start() {
}

void EditorGui::Init() {
    m_widgets.clear();
    const Size default_size = Size(.1, .1);
#define BUTTON(name,pos,size,type) BrushButtonPtr(new BrushButton(Sprite::GetByName(name), pos, size, Brush::New(Sprite::GetByName(name), type)))
#define ADD_BUTTON(name,pos,size,type) m_widgets.push_back(BUTTON(name,pos,size,type))
    // nazwy sprite'ów zdefiniowane są w SpriteConfig::SpriteConfig
    ADD_BUTTON("gui_eraser",       Position( 0, .0), default_size*2, Brush::ST::Eraser);

    ADD_BUTTON("player_stop",      Position(.1, .8), default_size, Brush::ST::Player);
    ADD_BUTTON("EndOfLevel",       Position(.2, .8), default_size, FT::EndOfLevel);
    ADD_BUTTON("twinshot_upgrade", Position(.4, .8), default_size, ET::TwinShot);
    ADD_BUTTON("mush_stop",        Position(.8, .8), default_size, ET::Mush);

    ADD_BUTTON("PlatformTopLeft",      Position(.3, .5),  default_size, FT::PlatformTopLeft);
    ADD_BUTTON("PlatformTop",          Position(.4, .5),  default_size, FT::PlatformTop);
    ADD_BUTTON("PlatformTopRight",     Position(.5, .5),  default_size, FT::PlatformTopRight);
    ADD_BUTTON("PlatformLeft",         Position(.3, .4),  default_size, FT::PlatformLeft); 
    ADD_BUTTON("PlatformMid",          Position(.4, .4),  default_size, FT::PlatformMid);
    ADD_BUTTON("PlatformRight",        Position(.5, .4),  default_size, FT::PlatformRight);
    ADD_BUTTON("PlatformLeftRight",    Position(.4, .15), default_size, FT::PlatformLeftRight);
    ADD_BUTTON("PlatformLeftTopRight", Position(.4, .25), default_size, FT::PlatformLeftTopRight);

#undef ADD_BUTTON
#undef BUTTON
}

void EditorGui::Draw() {
    // ciemne tło
    Engine::Get().GetRenderer()->DrawQuad(0, 0, 1, 1, 0,0,0,.7);

    // widoczne kontrolki
    for (BrushButtonContrainer::const_iterator i=m_widgets.begin(); i!=m_widgets.end(); ++i) {
        if ((*i)->IsVisible() && (*i)!=m_hovered_button) {
            (*i)->Draw();
        }
    }
    
    // "najechany" przycisk rysujemy osobno.
    if (m_hovered_button) {
        m_hovered_button->Draw();
        const Aabb& box = m_hovered_button->GetAabb();
        Engine::Get().GetRenderer()->DrawQuad(box.GetMinX(), box.GetMinY(),
                                              box.GetMaxX(), box.GetMaxY(),
                                              1,1,1, .4);
    }
}

void EditorGui::Update(double dt) {
    std::for_each(m_widgets.begin(), m_widgets.end(), boost::bind(&GuiWidget::Update, _1, dt));
}

bool EditorGui::OnMouseMove(double x, double y) {
    m_hovered_button.reset();
    const Aabb cursor_aabb = Aabb(x, y, x+.02, y+0.02);
    for (BrushButtonContrainer::const_iterator i=m_widgets.begin(); i!=m_widgets.end(); ++i) {
        if ((*i)->IsVisible() && (*i)->GetAabb().Collides(cursor_aabb)) {
            m_hovered_button = *i;
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