#ifndef __WINDOW_H__
#define __WINDOW_H__

class Window {
public:
    explicit Window() {
        m_width = 0;
        m_height = 0;
    }

    size_t GetWidth() const {
        return m_width;
    }

    size_t GetHeight() const {
        return m_height;
    }

    void SetSize(size_t width, size_t height) {
        m_width = width;
        m_height = height;
    }

private:
    size_t m_width;
    size_t m_height;
};

typedef boost::shared_ptr<Window> WindowPtr;

#endif /* __WINDOW_H__ */
