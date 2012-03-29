#include "StdAfx.h"

#include "ScoreSubmit.h"
#include "video/Text.h"
#include "Engine.h"
#include "HallOfFame.h"
#include "Utils.h"


ScoreSubmit::ScoreSubmit(size_t points) 
    : m_is_done(false),
      m_player_nickname("__________"),
      m_next_letter(0),
      m_points(points),
      m_highlighted_char(' ') {
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
    
    t.DrawText(m_player_nickname, 0.25, 0.6);
    
    t.SetSize(0.05, 0.05);
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        std::pair<double, double> pos = LetterPosition(ch);
        if (ch == m_highlighted_char) {
            Engine::Get().GetRenderer()->DrawQuad(pos.first - 0.005, pos.second - 0.005, 
                                                  pos.first + 0.055, pos.second + 0.055,
                                                  1,0,0,1);
        }
        t.DrawLetter(ch, pos.first, pos.second);
    }

    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}

std::pair<double, double> ScoreSubmit::LetterPosition(char ch) {
    int index = ch - 'a';
    int col = index % 7,
        row = index / 7;

    return std::make_pair(0.25 + col * 0.07, 
                          0.45 - row * 0.07);
}

bool ScoreSubmit::Update(double /* dt */) {
    return !IsDone();
}

void ScoreSubmit::ProcessEvents(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        SetDone();
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN && m_player_nickname.at(0) != '_') {
            StoreInFile();
            SetDone();
        }
        else if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z && m_next_letter < m_player_nickname.size()) { 
            char key = event.key.keysym.sym - SDLK_a + 'a';
            m_player_nickname.at(m_next_letter++) = key;
        }
        else if (event.key.keysym.sym == SDLK_BACKSPACE) {
            if (m_player_nickname.at(0) != '_') {
                m_player_nickname.at(--m_next_letter) = '_';
            }
        }
    }
    else if (event.type == SDL_MOUSEMOTION) {
        double x = event.motion.x / static_cast<double>(Engine::Get().GetWindow()->GetWidth());
        double y = 1.0 - event.motion.y / static_cast<double>(Engine::Get().GetWindow()->GetHeight());
        m_highlighted_char = ' ';
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            std::pair<double, double> ch_pos = LetterPosition(ch);
            if (x >= ch_pos.first && x <= ch_pos.first + 0.07
                && y <= ch_pos.second + 0.07 && y >= ch_pos.second) {
                m_highlighted_char = ch;
            }
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (m_highlighted_char != ' ' && m_next_letter < m_player_nickname.size()) {
            m_player_nickname.at(m_next_letter++) = m_highlighted_char;
        }
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
        BOOST_FOREACH(char c, m_player_nickname) {
            if (c != '_') {
                player_entry.name += c;
            }
        }
        entries.push_back(player_entry);
    }
    {
        const unsigned max_records_to_save = 10;
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
}

void ScoreSubmit::Start() {
    Engine::Get().GetSound()->PlayMusic("ss");
}

AppStatePtr ScoreSubmit::NextAppState() const {
    return AppStatePtr(new HallOfFame);
}
