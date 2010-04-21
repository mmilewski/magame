#include "StdAfx.h"

#include "Game.h"

class App {
public:
    explicit App(bool fullscreen_mode) 
        : m_fullscreen(fullscreen_mode) {
    }

    void Run();

private:
    void Draw();
    void Update(double dt);
    void Resize(size_t width, size_t height);
    void ProcessEvents();

private:
    bool m_fullscreen;       // tryb pełnoekranowy
    SDL_Surface* m_screen;

    AppStatePtr m_app_state;
};

