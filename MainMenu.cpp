#include "StdAfx.h"

#include "MainMenu.h"
#include "video/Text.h"
#include "Engine.h"
#include "LevelChoiceScreen.h"
#include "HallOfFame.h"
#include "TransitionEffect.h"
#include "Level.h"
#include "editor/Editor.h"
#include "editor/EditorLevelChoice.hpp"
#include "video/Rgba.h"

void MainMenu::Init() {

}

void MainMenu::Start() {
    Engine::Get().GetSound()->PlayMusic("menu");
}

void MainMenu::Draw() {
    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }

    Text t(0.1, 0.1);
    t.DrawText("menu", 0.3, 0.8);
    
    const double yStart = 0.6, yDelta = 0.1;
    auto miny = [&](int which) { return yStart - yDelta * which - .01; };
    auto maxy = [&](int which) { return yStart - yDelta * which + .06; };

    Rgba selColor = Rgba(.3, 0.8, 0.2, .5);
    if (m_selection == Sel::NewGame) {
        Engine::Get().GetRenderer()->DrawQuad(0.3, miny(0), 0.72, maxy(0), selColor);
    }
    else if (m_selection == Sel::HallOfFame) {
        Engine::Get().GetRenderer()->DrawQuad(0.2, miny(1), 0.82, maxy(1), selColor);
    }
    else if (m_selection == Sel::Editor) {
        Engine::Get().GetRenderer()->DrawQuad(0.35, miny(2), 0.67, maxy(2), selColor);
    }
    else if (m_selection == Sel::Quit) {
        Engine::Get().GetRenderer()->DrawQuad(0.325, miny(3), 0.695, maxy(3), selColor);
    }

    t.SetSize(0.05, 0.05);
    t.DrawText("nowa gra",     0.31, yStart - yDelta * 0);
    t.DrawText("hall of fame", 0.21, yStart - yDelta * 1);
    t.DrawText("edytor",       0.36, yStart - yDelta * 2);
    t.DrawText("wyjscie",      0.33, yStart - yDelta * 3);

    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}

bool MainMenu::Update(double /* dt */) {
    return !IsDone();
}

void MainMenu::ProcessEvents(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        SetDone();
    }
    else if (event.type == SDL_KEYDOWN) { 
        if (event.key.keysym.sym == SDLK_DOWN) {
            m_selection = Sel::Selection((m_selection + 1));
            if (m_selection > Sel::Quit) {
                m_selection = Sel::NewGame;
            }
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            m_selection = Sel::Selection(m_selection - 1);
            if (m_selection < Sel::NewGame) {
                m_selection = Sel::Quit;
            }
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {
            if (m_selection == Sel::NewGame) {
//                m_next_app_state.reset(new Game("1", PlayerPtr()) );
                m_next_app_state.reset(new LevelChoiceScreen(PlayerPtr()));
            }
            else if (m_selection == Sel::HallOfFame) {
                HallOfFamePtr next_state = HallOfFame::New();
                
                tefPtr fadein = TransitionEffect::PrepareFadeIn(next_state).duration(1).Build();
                tefPtr fadeout = TransitionEffect::PreparePinWheelOut().states(shared_from_this(),fadein).duration(1.5).blades(2).rotation(90).delay(0,.3).Build();
                m_next_app_state = fadeout;
            }
            else if (m_selection == Sel::Editor) {
                AppStatePtr editorLevelChoice(new EditorLevelChoice);
                tefPtr fadeout = TransitionEffect::PreparePinWheelOut().states(shared_from_this(),editorLevelChoice).duration(1.0).blades(2).delay(0,.2).Build();
                m_next_app_state = fadeout;
            }
            else if (m_selection == Sel::Quit) {
                m_next_app_state.reset();
            }

            SetDone();
        }
        else if (event.key.keysym.sym == SDLK_ESCAPE) {
//            TransitionEffectPtr fadeout = TransitionEffect::Prepare(TransitionEffectType::FadeOut).from(shared_from_this()).Build();
            TransitionEffectPtr fadeout = TransitionEffect::PrepareFadeOut(shared_from_this()).Build();
            m_next_app_state = fadeout;
            SetDone();
        }
    }
}

boost::shared_ptr<AppState> MainMenu::NextAppState() const {
    return m_next_app_state;
}
