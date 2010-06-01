#include "StdAfx.h"
#include "FadeEffect.h"
#include "Engine.h"


FadeEffect::FadeEffect(double delay, double duration, AppStatePtr from_state, AppStatePtr to_state, FadeEffectType::Type effect_type)
    : m_delay(delay), 
      m_duration(duration), 
      m_from_state(from_state),
      m_to_state(to_state),
      m_effect_type(effect_type),
      m_alpha(0),
      m_timer(0) {

    // TODO: kolor?
    // Ka¿dy stan powinien mieæ shared_from_this

  m_delay = 0;
}

FadeEffectPtr FadeEffect::New(double delay, double duration, AppStatePtr from_state, AppStatePtr to_state, FadeEffectType::Type effect_type) {
    return FadeEffectPtr(new FadeEffect(delay, duration, from_state, to_state, effect_type));
}

void FadeEffect::Start() {
    m_timer = 0;
    if (m_effect_type==FadeEffectType::FadeIn) {
        m_alpha = 1;   // du¿o czarnego i bêdzie coraz mniej
    } else if (m_effect_type==FadeEffectType::FadeOut) {
        m_alpha = 0;   // ma³o czarnego i bêdzie coraz wiêcej
    } else {
        assert(false && "Start: Nieznany typ efektu");
    }
}

void FadeEffect::Init() {
    // nie potrzebujemy ¿adnych zasobów
}

void FadeEffect::Draw() {
    if (IsDone()) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    std::cout << m_timer << "\t" << m_alpha << std::endl;
    if (m_from_state) {
        m_from_state->m_clear_before_draw = false;
        m_from_state->m_swap_after_draw = false;
        m_from_state->Draw();
    }
    
    Engine& engine = Engine::Get();
    engine.GetRenderer()->DrawQuad(0,0,1,1, 0,0,0,m_alpha);

    SDL_GL_SwapBuffers();
}

bool FadeEffect::Update(double dt) {
    // je¿eli efekt siê zakoñczy³ to nic nie robimy
    if (IsDone() || (m_timer >= m_delay + m_duration)) {
        SetDone(true);
        return IsDone();
    }
    m_timer += dt;

    // efekt jest aktywny tylko jeœli up³yn¹³ czas delay
    if (m_timer >= m_delay) {
        if (m_effect_type==FadeEffectType::FadeIn) {
            m_alpha -= dt/m_duration;
        } else if (m_effect_type==FadeEffectType::FadeOut) {
            m_alpha += dt/m_duration;
        } else {
            assert(false && "Update: Nieznany typ efektu");
        }
    }
    return !IsDone();
}

void FadeEffect::ProcessEvents(const SDL_Event& event) {
}

AppStatePtr FadeEffect::NextAppState() const {
    if (IsDone()) {
        if (m_from_state) {
            m_from_state->m_clear_before_draw = true;
            m_from_state->m_swap_after_draw = true;
        }
        return m_to_state;
    } else {
        return AppStatePtr();
    }
}
