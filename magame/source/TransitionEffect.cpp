#include "StdAfx.h"
#include "TransitionEffect.h"
#include "Engine.h"


TransitionEffect::TransitionEffect(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, double delay)
    : m_delay(delay), 
      m_duration(duration), 
      m_from_state(from_state),
      m_to_state(to_state),
      m_effect_type(effect_type),
      m_alpha(0),
      m_timer(0) {

    // TODO: kolor?
    // Każdy stan powinien mieć shared_from_this

  m_delay = 0;
}

TransitionEffectPtr TransitionEffect::New(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, double delay) {
    return TransitionEffectPtr(new TransitionEffect(from_state, to_state, effect_type, duration, delay));
}

TransitionEffectPtr TransitionEffect::NewFadeIn(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay) {
    return TransitionEffectPtr(new TransitionEffect(from_state, to_state, TransitionEffectType::FadeIn, duration, delay));
}

TransitionEffectPtr TransitionEffect::NewFadeOut(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay) {
    return TransitionEffectPtr(new TransitionEffect(from_state, to_state, TransitionEffectType::FadeOut, duration, delay));
}

void TransitionEffect::Start() {
    m_timer = 0;
    if (m_effect_type==TransitionEffectType::FadeIn) {
        m_alpha = 1;   // dużo czarnego i będzie coraz mniej
    } else if (m_effect_type==TransitionEffectType::FadeOut) {
        m_alpha = 0;   // mało czarnego i będzie coraz więcej
    } else {
        assert(false && "Start: Nieznany typ efektu");
    }
}

void TransitionEffect::Init() {
    // nie potrzebujemy żadnych zasobów
}

void TransitionEffect::Draw() {
    if (IsDone()) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (m_effect_type==TransitionEffectType::FadeIn) {
        if (m_to_state) {
            m_to_state->SetClearBeforeDraw(false)->SetSwapAfterDraw(false);
            m_to_state->Draw();
        }
    } else if (m_effect_type==TransitionEffectType::FadeOut) {
        if (m_from_state) {
            m_from_state->SetClearBeforeDraw(false)->SetSwapAfterDraw(false);
            m_from_state->Draw();
        }
    } else {
        assert(false && "Draw: Nieznany typ efektu");
    }

    Engine& engine = Engine::Get();
    engine.GetRenderer()->DrawQuad(0,0,1,1, 0,0,0, m_alpha);

    SDL_GL_SwapBuffers();
}

bool TransitionEffect::Update(double dt) {
    // jeżeli efekt się zakończył to nic nie robimy
    if (IsDone() || (m_timer >= m_delay + m_duration)) {
        SetDone(true);
        return IsDone();
    }
    m_timer += dt;

    // efekt jest aktywny tylko jeśli upłynął czas delay
    if (m_timer >= m_delay) {
        if (m_effect_type==TransitionEffectType::FadeIn) {
            m_alpha -= dt/m_duration;
        } else if (m_effect_type==TransitionEffectType::FadeOut) {
            m_alpha += dt/m_duration;
        } else {
            assert(false && "Update: Nieznany typ efektu");
        }
    }
    return !IsDone();
}

void TransitionEffect::ProcessEvents(const SDL_Event& event) {
}

AppStatePtr TransitionEffect::NextAppState() const {
    if (IsDone()) {
        if (m_from_state) {
            m_from_state->SetClearBeforeDraw(true)->SetSwapAfterDraw(true);
        }
        return m_to_state;
    } else {
        return AppStatePtr();
    }
}
