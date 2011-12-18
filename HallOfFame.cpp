#include "StdAfx.h"

#include "HallOfFame.h"
#include "Text.h"
#include "Engine.h"
#include "MainMenu.h"


HallOfFame::HallOfFame() 
    : m_is_done(false) {
    LoadFromFile();
}

HallOfFamePtr HallOfFame::New() {
    return HallOfFamePtr(new HallOfFame);
}

void HallOfFame::Draw() {
    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }

    Text t(0.08, 0.08);
    t.DrawText("Hall Of Fame", 0.01, 0.9);

    t.SetSize(0.035, 0.035);
    double x = 0.15, y = 0.7;
    for (size_t i = 0; i < m_entries.size(); ++i) {
        t.DrawText(m_entries.at(i).name, x, y);
        t.DrawNumber(m_entries.at(i).points, x+0.4, y, 8);
        y -= 0.07;
    }

    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}

bool HallOfFame::Update(double /* dt */) {
    return !IsDone();
}

void HallOfFame::ProcessEvents(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        SetDone();
    }
}

void HallOfFame::Init() {
}

void HallOfFame::Start() {
}

void HallOfFame::LoadFromFile() {
    std::ifstream in("data/hof.txt");
    if (!in) {
        std::cerr << "Nie moge odczytac Hall of Fame\n";
        return;
    }

    Entry entry;
    while (in >> entry.name >> entry.points) {
        m_entries.push_back(entry);
    }
}

AppStatePtr HallOfFame::NextAppState() const {
    return AppStatePtr(new MainMenu);
}
