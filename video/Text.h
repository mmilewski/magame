#ifndef MAGAME_TEXT_H_INCLUDED
#define MAGAME_TEXT_H_INCLUDED
#include "StdAfx.h"
#include "math/BasicMathTypes.h"

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

    void DrawDigit(char ch, double pos_x, double pos_y) const;
    void DrawLetter(char ch, Position pos) const;
    void DrawSpecial(char ch, double pos_x, double pos_y) const;
    void DrawText(const std::string& text, double pos_x, double pos_y) const;
    void DrawText(std::string const& text, Position pos) const;
    void DrawNumber(size_t number, double pos_x, double pos_y, size_t width = 0) const;

private:
    void Draw(double tex_x, double tex_y, double pos_x, double pos_y) const;

private:
    double m_width;
    double m_height;
    size_t m_layer;
};

#endif /* MAGAME_TEXT_H_INCLUDED */
