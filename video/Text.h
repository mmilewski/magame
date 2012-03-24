#ifndef MAGAME_TEXT_H_INCLUDED
#define MAGAME_TEXT_H_INCLUDED
#include "StdAfx.h"

class Text {
public:
    explicit Text(double width = 0.025, double height = 0.025, size_t layer = 0) {
        SetSize(width, height);
        SetLayer(layer);
    }

    void SetSize(double width, double height) {
        m_width = width;
        m_height = height;
    }

    void SetLayer(size_t layer) {
        m_layer = layer;
    }

    void DrawDigit(char ch, double pos_x, double pos_y);
    void DrawLetter(char ch, double pos_x, double pos_y);
    void DrawSpecial(char ch, double pos_x, double pos_y);
    void DrawText(const std::string& text, double pos_x, double pos_y);
    void DrawNumber(size_t number, double pos_x, double pos_y, size_t width = 0);

private:
    void Draw(double tex_x, double tex_y, double pos_x, double pos_y);

private:
    double m_width;
    double m_height;
    size_t m_layer;
};

#endif /* MAGAME_TEXT_H_INCLUDED */
