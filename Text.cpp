#include "StdAfx.h"

#include "Utils.h"
#include "Engine.h"
#include "Text.h"

void Text::DrawDigit(char ch, double pos_x, double pos_y) {
    int digit = ch - '0';
    int tex_x = digit * 32;
    int tex_y = 8*32;
    Draw(tex_x, tex_y, pos_x, pos_y);
}

void Text::DrawLetter(char ch, Position pos) {
    int letter = toupper(ch) - 'A';
    
    int letter_row = letter / 10; // wiersz, w którym jest litera
    int letter_col = letter % 10; // kolumna, w której jest litera

    int tex_x = letter_col * 32;
    int tex_y = (9+letter_row) * 32;

    Draw(tex_x, tex_y, pos.X(), pos.Y());
}

void Text::DrawSpecial(char ch, double pos_x, double pos_y) {
    double tex_x = 0;
    double tex_y = 0;

    if (ch == '_') {
        tex_x = 192;
        tex_y = 352;
    }
    else {
        return; // pomijamy znaki, których nie znamy
    }
    Draw(tex_x, tex_y, pos_x, pos_y);
}

void Text::DrawText(const std::string& text, double pos_x, double pos_y) {
    double x = pos_x;
    double y = pos_y;

    for (size_t i = 0; i < text.size(); ++i) {
        char ch = text.at(i);
        if (isdigit(ch)) {
            DrawDigit(ch, x, y);
        }
        else if (isalpha(ch)) {
            DrawLetter(ch, Position(x, y));
        }
        else if (ch == '_') {
            DrawSpecial(ch, x, y);
        }
        else {
            ; // inne znaki po prostu pomijamy
        }
        x += m_width;
    }
}

void Text::DrawText(std::string const& text, Position pos) {
    DrawText(text, pos.X(), pos.Y());
}

void Text::DrawNumber(size_t number, double pos_x, double pos_y, size_t width) {
    std::string number_str = IntToStr(number);
    size_t spaces_count = std::max(0, static_cast<int> (width) - static_cast<int> (number_str.size()));
    for (size_t i = 0; i < spaces_count; ++i) {
        number_str = " " + number_str;
    }
    DrawText(number_str, pos_x, pos_y);
}

void Text::Draw(double tex_x, double tex_y, double pos_x, double pos_y) {
    tex_x = static_cast<int>(tex_x);  // "zaokrąglanie"
    tex_y = static_cast<int>(tex_y);

    glPushMatrix(); // MODELVIEW
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        {
            Engine::Get().GetRenderer()->ResetProjection();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            Engine::Get().GetRenderer()->DrawSprite(tex_x, tex_y, 32, 32, 
                                                 pos_x, pos_y, m_width, m_height,
                                                 DL::DisplayLayer(m_layer));
        }
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
