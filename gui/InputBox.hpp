#ifndef MAGAME_INPUTBOX_HPP_INCLUDED
#define MAGAME_INPUTBOX_HPP_INCLUDED
#include "StdAfx.h"

#include "Widget.h"

namespace gui {

class InputBox : public Widget
{
public:
    explicit InputBox(Position position);

    virtual void Draw() const;
    virtual void Update(double dt);

    std::string GetText() const { return m_text; }
    void PushChar(char c);
    void PopChar();
    void OnMouseMove(int mouse_x, int mouse_y);
    void OnMouseDown(int mouse_x, int mouse_y);

private:
    Position LetterPosition(char ch) const;

    const size_t m_max_text_length;
    const double m_letterbox_size;
    const size_t m_letters_in_row;
    std::string m_text;
    char m_highlighted_char;
};

} // namespace gui

#endif // MAGAME_INPUTBOX_HPP_INCLUDED
