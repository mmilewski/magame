#include "StdAfx.h"

#include "MainMenu.h"
#include "Text.h"
#include "Engine.h"
//#include "Game.h"
#include "LevelChoiceScreen.h"
#include "HallOfFame.h"
#include "TransitionEffect.h"


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
    
    if (m_selection == Sel::NewGame) {
        Engine::Get().GetRenderer()->DrawQuad(0.3, 0.59, 0.72, 0.66,  .3, 0.8, 0.2, .5);
    }
    else if (m_selection == Sel::HallOfFame) {
        Engine::Get().GetRenderer()->DrawQuad(0.2, 0.49, 0.82, 0.56,  .3, 0.8, 0.2, .5);
    }
    else if (m_selection == Sel::Quit) {
        Engine::Get().GetRenderer()->DrawQuad(0.325, 0.39, 0.695, 0.46,  .3, 0.8, 0.2, .5);
    }

    t.SetSize(0.05, 0.05);
    t.DrawText("nowa gra", 0.31, 0.6);
    t.DrawText("hall of fame", 0.21, 0.5);
    t.DrawText("wyjscie", 0.335, 0.4);

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
                
                tefPtr fadein = TransitionEffect::Prepare(TransitionEffectType::FadeIn).to(next_state).duration(1).Build();
                tefPtr fadeout = TransitionEffect::Prepare(TransitionEffectType::PinWheelOut).states(shared_from_this(),fadein).duration(1.5).blades(2).rotation(90).delay(0,.3).Build();
                m_next_app_state = fadeout;
            }
            else if (m_selection == Sel::Quit) {
                m_next_app_state.reset();
            }

            SetDone();
        }
        else if (event.key.keysym.sym == SDLK_ESCAPE) {
            TransitionEffectPtr fadeout = TransitionEffect::Prepare(TransitionEffectType::FadeOut).from(shared_from_this()).Build();
            m_next_app_state = fadeout;
//            m_next_app_state.reset();
            SetDone();
        }
    }
}

boost::shared_ptr<AppState> MainMenu::NextAppState() const {
    return m_next_app_state;
}
