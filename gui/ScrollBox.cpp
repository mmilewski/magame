#include "StdAfx.h"
#include "ScrollBox.hpp"
#include "Engine.h"
#include "Renderer.h"
#include "Text.h"
#include "Utils.h"

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
    RendererPtr renderer = Engine::Get().GetRenderer();

    // config values
    const int display_before_selected = 2, display_after_selected = 3;
    const double item_height = 0.07, font_height = 0.05;

    // caption
    Text(0.07, 0.1).DrawText("Select level", 0.05, 0.8);

    // few useful values
    Text text(0.05, font_height);
    const int sel_id = static_cast<int>(SelectedId());
    const double sel_y = (GetPosition() + GetSize()/2).Y();  // selection is widget-centered
    const double centered_sel_y = sel_y + (item_height-font_height)/2; // text is selection-centered

    // draw highlight (background for selected item)
    const Position highlight_pos = Position(GetPosition().X(), sel_y-0.01);
    const Size highlight_size = Size(GetSize().X(), item_height+0.02);
    renderer->DrawQuad(highlight_pos, highlight_pos + highlight_size, 0,1,0, .5);

    // draw selected item
    text.DrawText(m_items.at(SelectedId()), GetPosition().X(), centered_sel_y);

    // draw items before selected
    for (int idx = sel_id - 1; (idx >= 0) && (idx+display_before_selected >= sel_id); --idx) {
        text.DrawText(m_items.at(idx),
                      GetPosition().X(),
                      centered_sel_y + item_height * (sel_id-idx));
    }

    // draw items after selected
    for (int idx = sel_id + 1; (idx < (int)m_items.size()) && (idx-sel_id <= display_after_selected); ++idx) {
        text.DrawText(m_items.at(idx),
                      GetPosition().X(),
                      centered_sel_y - item_height * (idx-sel_id));
    }
}

void ScrollBox::Update(double /*dt*/)
{

}

} // namespace gui
