#include "StdAfx.h"
#include "ScrollBox.hpp"
#include "Engine.h"
#include "video/Renderer.h"
#include "video/Text.h"
#include "common/Utils.h"

namespace gui {

ScrollBox::ScrollBox(Position position, Size size) :
    GuiWidget(position, size, true/*visible*/),
    m_selected_id(0)
{
}

void ScrollBox::Draw() const
{
    if (m_items.empty()) {
        return;
    }
    const int display_before = 2, display_after = 2;
    const double itemHeight = 0.07;
    RendererPtr renderer = Engine::Get().GetRenderer();

    Text(0.07, 0.1).DrawText("Select level", 0.05, 0.8);
    Text text(0.05, 0.05);
    double sel_y = (GetSizedPosition().Y() - GetPosition().Y())/2 + GetPosition().Y();
    const Position highlight = Position(GetPosition().X(), sel_y-0.02);
    renderer->DrawQuad(highlight, highlight+Size(GetSize().X(),itemHeight+0.02), Rgba(0,1,0, .5));
    text.DrawText(m_items.at(SelectedId()), GetPosition().X(), sel_y);
    const int sel_id = static_cast<int>(SelectedId());
    for (int idx = sel_id - 1; (idx >= 0) && (idx+display_before >= sel_id); --idx) {
        text.DrawText(m_items.at(idx), GetPosition().X(), sel_y + itemHeight * (sel_id-idx));
    }
    for (int idx = sel_id + 1; (idx < (int)m_items.size()) && (idx-sel_id <= display_after); ++idx) {
        text.DrawText(m_items.at(idx), GetPosition().X(), sel_y - itemHeight * (idx-sel_id));
    }
}

void ScrollBox::Update(double /*dt*/)
{

}

} // namespace gui
