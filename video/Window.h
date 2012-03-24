#ifndef MAGAME_WINDOW_H_INCLUDED
#define MAGAME_WINDOW_H_INCLUDED
#include "StdAfx.h"

class Window {
public:
    explicit Window() : m_width(0), m_height(0), m_is_fullscreen(false) {
    }

    size_t GetWidth() const {
        return m_width;
    }

    size_t GetHeight() const {
        return m_height;
    }

    bool IsFullscreen() const {
        return m_is_fullscreen;
    }

    void SetSize(size_t width, size_t height) {
        m_width = width;
        m_height = height;
    }

    void SetFullscreen(bool fullscreen_mode) {
        m_is_fullscreen = fullscreen_mode;
    }

private:
    size_t m_width;
    size_t m_height;
    bool m_is_fullscreen;
};

typedef boost::shared_ptr<Window> WindowPtr;

#endif /* MAGAME_WINDOW_H_INCLUDED */
