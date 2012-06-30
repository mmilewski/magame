#ifndef MAGAME_SCROLL_BOX_HPP_INCLUDED
#define MAGAME_SCROLL_BOX_HPP_INCLUDED
#include "StdAfx.h"
#include "gui/Widget.h"
#include "video/Renderer.h"

namespace gui {

class ScrollBox : public Widget
{
public:
    ScrollBox(Position position, Size size);

    virtual void Draw() const;
    virtual void Update(double dt);

    void SelectPrev() { if (m_selected_id) m_selected_id--; }
    void SelectNext() { if (m_selected_id+1 < m_items.size()) m_selected_id++; }
    size_t SelectedId() const { return m_selected_id; }
    bool HasSelection() const { return !m_items.empty(); }

    void SetItems(std::deque<std::string> const& items) { m_items = items; }
private:
    std::deque<std::string> m_items;
    size_t m_selected_id;
};


} // namespace gui

#endif // MAGAME_SCROLL_BOX_HPP_INCLUDED
