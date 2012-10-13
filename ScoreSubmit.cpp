#include "StdAfx.h"

#include "ScoreSubmit.h"
#include "video/Text.h"
#include "Engine.h"
#include "HallOfFame.h"
#include "common/Utils.h"


ScoreSubmit::ScoreSubmit(size_t points) :
    m_points(points)
{
    SetDone(false);
}

void ScoreSubmit::Draw() const {
    Text t(0.08, 0.08);
    t.DrawText("gratulacje", 0.1, 0.9);
    t.SetSize(0.04, 0.04);
    t.DrawText("punkty: " + IntToStr(m_points), 0.1, 0.8);
    t.SetSize(0.05, 0.05);
    t.DrawText("wpisz swoje imie", 0.1, 0.7);
    
    m_name_input->Draw();
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
    std::vector<Entry> entries;

    {
        std::ifstream hofReader("data/hof.txt");
        if (!hofReader) {
            std::cerr << "Nie moge odczytac Hall of Fame\n";
            return;
        }
        Entry e;
        while (hofReader >> e.name >> e.points) {
            entries.push_back(e);
        }
        hofReader.close();
    }
    {
        Entry player_entry;
        player_entry.points = m_points;
        player_entry.name = m_name_input->GetText();
        entries.push_back(player_entry);
    }
    {
        const size_t max_records_to_save = 10;
        boost::sort(entries, [](Entry a, Entry b){return a.points > b.points;});
        entries.resize(std::min(max_records_to_save, entries.size()));
        std::ofstream hofWriter("data/hof.txt");
        BOOST_FOREACH(const auto& entry, entries) {
            hofWriter << entry.name << " " << entry.points << "\n";
        }
        hofWriter.close();
    }
}

void ScoreSubmit::Init() {
    m_name_input.reset(new gui::InputBox(Position(0.25, 0.45)));
}

void ScoreSubmit::Start() {
    Engine::Get().GetSound()->PlayMusic("ss");
}

AppStatePtr ScoreSubmit::NextAppState() const {
    return AppStatePtr(new HallOfFame);
}
