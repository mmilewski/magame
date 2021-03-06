#include "StdAfx.h"

#include "HallOfFame.h"
#include "video/Text.h"
#include "Engine.h"
#include "MainMenu.h"


HallOfFame::HallOfFame()
    : m_is_done(false) {
    LoadFromFile();
}

HallOfFamePtr HallOfFame::New() {
    return HallOfFamePtr(new HallOfFame);
}

void HallOfFame::Draw() const {
    Text t(0.08, 0.08);
    t.DrawText("Hall Of Fame", 0.01, 0.9);

    t.SetSize(0.035, 0.035);
    double x = 0.15, y = 0.7;

    for(auto& entry : m_entries) {
        t.DrawText(entry.name, x, y);
        t.DrawNumber(entry.points, x+0.4, y, 8);
        y -= 0.07;
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
