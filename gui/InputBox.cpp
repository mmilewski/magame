#include "InputBox.hpp"
#include "Text.h"
#include "Engine.h"

InputBox::InputBox(Position position) :
    GuiWidget(position, Size(.5,.5), true/*visible*/),  // size is not useful, now
    m_max_text_length(10),
    m_letterbox_size(0.07),
    m_letters_in_row(7),
    m_text(""),
    m_highlighted_char('\0')
{
}

Position InputBox::LetterPosition(char ch) const {
    int index = ch - 'a';
    int col = index % m_letters_in_row,
        row = index / m_letters_in_row;
    return GetPosition() + Position(col, -row) * m_letterbox_size;
}

void InputBox::Draw() const {
    Text t(0.05, 0.05);
    {
        std::string input_text = GetText();
        input_text.resize(m_max_text_length, '_');
        t.DrawText(input_text, GetPosition() + Position(0, .1));
    }
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        Position pos = LetterPosition(ch);
        if (ch == m_highlighted_char) {
            // Little hack here, since it's hard to print a char centered in a box
            Position min_position = pos - Size(0.005, 0.005);
            Position max_position = min_position + Size(m_letterbox_size, m_letterbox_size) * .9;
            Engine::Get().GetRenderer()->DrawQuad(
                        min_position, max_position,
                        1,0,0,1);
        }
        t.DrawLetter(ch, pos);
    }
}

void InputBox::Update(double /*dt*/) {
}

void InputBox::PushChar(char c) {
    if (m_text.size() < m_max_text_length) {
        m_text.push_back(c);
    }
}

void InputBox::PopChar() {
    if (!m_text.empty()) {
        m_text.resize(m_text.size() - 1);
    }
}

void InputBox::OnMouseMove(int mouse_x, int mouse_y) {
    auto window = Engine::Get().GetWindow();
    const double x = mouse_x / double(window->GetWidth());
    const double y = 1.0 - mouse_y / double(window->GetHeight());
    m_highlighted_char = '\0';
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        Position ch_pos = LetterPosition(ch);
        if (ch_pos.X() < x && x < ch_pos.X() + m_letterbox_size
         && ch_pos.Y() < y && y < ch_pos.Y() + m_letterbox_size) {
            m_highlighted_char = ch;
        }
    }
}

void InputBox::OnMouseDown(int /*mouse_x*/, int /*mouse_y*/) {
    if (m_highlighted_char) {
        PushChar(m_highlighted_char);
    }
}
