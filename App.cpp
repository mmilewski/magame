#include <cassert>

#include "App.h"
#include "Engine.h"
#include "MainMenu.h"
#include "HallOfFame.h"

void App::ProcessEvents() {
    // przyjrzyj zdarzenia
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_VIDEORESIZE) {
            Resize(event.resize.w, event.resize.h);
        } else {
            m_app_state->ProcessEvents(event);
        }
    }
}

void App::Run() {
    // inicjalizacja okna
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Resize(Engine::Get().Window()->GetWidth(),
           Engine::Get().Window()->GetHeight());
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // podwójne buforowanie

    Engine::Get().Sound()->LoadSounds();

    // inicjalizacja OpenGL
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST); // niewyświetlanie przezroczystych fragmentów sprite'a
    glAlphaFunc(GL_GEQUAL, 0.1);

    // ładowanie atlasu
    const std::string atlas_filename = "data/tex.bmp";
    Engine::Get().Renderer()->LoadTexture(atlas_filename);

    m_app_state.reset(new MainMenu);
    m_app_state->Init();
    m_app_state->Start();

    // pętla główna
    size_t last_ticks = SDL_GetTicks();
    while (true) {
        if (m_app_state->IsDone()) {
            m_app_state = m_app_state->NextAppState();
            if (!m_app_state) {
                return;
            }
            m_app_state->Init();
            m_app_state->Start();
        }

        ProcessEvents();

        // time update
        size_t ticks = SDL_GetTicks();
        double delta_time = (ticks - last_ticks) / 1000.0;
        last_ticks = ticks;

        // update & render
        if (delta_time > 0) {
            // std::cout << "delta_time: " << 1.0/delta_time << std::endl;
            Update(delta_time);
        }
        Draw();
    }

    SDL_Quit();
}

void App::Update(double dt) {
    m_app_state->Update(dt);
}

void App::Draw() {
    m_app_state->Draw();
}

void App::Resize(size_t width, size_t height) {
    m_screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_RESIZABLE | SDL_HWSURFACE);
    assert(m_screen && "problem z ustawieniem wideo");

    Engine::Get().Window()->SetSize(width, height);

    Engine::Get().Renderer()->SetProjection(width, height);
}
