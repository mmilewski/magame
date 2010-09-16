#include "StdAfx.h"

#include "Engine.h"
#include "Gui.h"

EditorGui::EditorGui() {
}

void EditorGui::Start() {
}

void EditorGui::Init() {
    m_widgets.clear();
    const Size default_size = Size(.1, .1);
#define BUTTON(sname,p,s,ft) SpriteButtonPtr(new SpriteButton(Sprite::GetByName(sname), p, s, Brush::New(Sprite::GetByName(sname), ft)))
    m_widgets.push_back(BUTTON("player_stop",      Position(.1, .8), default_size, Brush::ST::Player));
    m_widgets.push_back(BUTTON("platform_left",    Position(.2, .8), default_size, FT::PlatformMidPart));
    m_widgets.push_back(BUTTON("twinshot_upgrade", Position(.3, .8), default_size, ET::TwinShot));
    m_widgets.push_back(BUTTON("mush_stop",        Position(.4, .8), default_size, ET::Mush));
#undef NEWBUTTON
}

void EditorGui::Draw() {
    // ciemne tło
    Engine::Get().GetRenderer()->DrawQuad(0, 0, 1, 1, 0,0,0,.7);

    // widoczne kontrolki
    for (ButtonContrainer::const_iterator i=m_widgets.begin(); i!=m_widgets.end(); ++i) {
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
    Aabb cursor_aabb = Aabb(x, y, x+.02, y+0.02);
    for (ButtonContrainer::const_iterator i=m_widgets.begin(); i!=m_widgets.end(); ++i) {
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
    // m_active_brush = Brush::New(FT::PlatformLeftEnd);
    return true;
}
