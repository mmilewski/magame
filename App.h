#include "StdAfx.h"

#include "AppState.h"

class App : boost::noncopyable {
public:
    explicit App(bool fullscreen_mode) 
        : m_fullscreen(fullscreen_mode),
          m_screen(NULL),
          m_app_state() {
    }

    void Run();

private:
    void Draw();
    void Update(double dt);
    void Resize(size_t width, size_t height);
    void ProcessEvents();

private:
    bool m_fullscreen;       // tryb pełnoekranowy
    std::unique_ptr<SDL_Surface> m_screen;

    AppStatePtr m_app_state;
};

