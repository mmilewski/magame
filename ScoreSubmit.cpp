#include "StdAfx.h"

#include "ScoreSubmit.h"
#include "Text.h"
#include "Engine.h"
#include "HallOfFame.h"
#include "Utils.h"


ScoreSubmit::ScoreSubmit(size_t points) :
    m_points(points)
{
    SetDone(false);
}

void ScoreSubmit::Draw() {
    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }

    Text t(0.08, 0.08);
    t.DrawText("gratulacje", 0.1, 0.9);
    t.SetSize(0.04, 0.04);
    t.DrawText("punkty: " + IntToStr(m_points), 0.1, 0.8);
    t.SetSize(0.05, 0.05);
    t.DrawText("wpisz swoje imie", 0.1, 0.7);
    
    m_name_input->Draw();

    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}

bool ScoreSubmit::Update(double dt) {
    m_name_input->Update(dt);
    return !IsDone();
}

void ScoreSubmit::ProcessEvents(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        SetDone();
    } else if (event.type == SDL_KEYDOWN) {
        SDLKey key = event.key.keysym.sym;
        if (key == SDLK_RETURN && !m_name_input->GetText().empty()) {
            StoreInFile();
            SetDone();
        }
        else if (SDLK_a <= key && key <= SDLK_z) {
            m_name_input->PushChar(key - SDLK_a + 'a');
        }
        else if (key == SDLK_BACKSPACE) {
            m_name_input->PopChar();
        }
    }
    else if (event.type == SDL_MOUSEMOTION) {
        m_name_input->OnMouseMove(event.motion.x, event.motion.y);
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        m_name_input->OnMouseDown(event.button.x, event.button.y);
    }
}

void ScoreSubmit::StoreInFile() {
    std::fstream hof("data/hof.txt");
    if (!hof) {
        std::cerr << "Nie moge odczytac Hall of Fame\n";
        return;
    }

    std::vector<Entry> entries;
    Entry e;
    while (hof >> e.name >> e.points) {
        entries.push_back(e);
    }

    Entry player_entry;
    player_entry.points = m_points;
    player_entry.name = m_name_input->GetText();
    entries.push_back(player_entry);

    int j = entries.size() - 1;
    while (j > 0) {
        if (entries.at(j-1).points < entries.at(j).points) {
            std::swap(entries.at(j-1), entries.at(j));
        }
        --j;
    }

    hof.close();
    hof.clear();

    hof.open("data/hof.txt", std::ios::out);
    for (size_t i = 0; i < 10 && i < entries.size(); ++i) {
        hof << entries.at(i).name << " " << entries.at(i).points << "\n";
    }
    hof.close();
}

void ScoreSubmit::Init() {
    m_name_input.reset(new InputBox(Position(0.25, 0.45)));
}

void ScoreSubmit::Start() {
    Engine::Get().GetSound()->PlayMusic("ss");
}

AppStatePtr ScoreSubmit::NextAppState() const {
    return AppStatePtr(new HallOfFame);
}
