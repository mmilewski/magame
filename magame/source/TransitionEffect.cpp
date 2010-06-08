#include "StdAfx.h"
#include "TransitionEffect.h"
#include "Engine.h"


TransitionEffect::TransitionEffect(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, double delay_before, double delay_after)
    : m_delay_before(delay_before), 
      m_delay_after(delay_after),
      m_duration(duration), 
      m_from_state(from_state),
      m_to_state(to_state),
      m_effect_type(effect_type),
      m_fade_alpha(0.0),
      m_quadric(0),
      m_sweep_angle(0.0),
      m_blades_count(1),
      m_timer(0.0) {

    // TODO: kolor?
    // Każdy stan powinien mieć shared_from_this
}

TransitionEffect::TransitionEffect(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, unsigned int blades, double delay_before, double delay_after)
    : m_delay_before(delay_before), 
      m_delay_after(delay_after),
      m_duration(duration), 
      m_from_state(from_state),
      m_to_state(to_state),
      m_effect_type(effect_type),
      m_fade_alpha(0.0),
      m_quadric(0),
      m_sweep_angle(0.0),
      m_blades_count(blades),
      m_timer(0.0) {

    // TODO: kolor?
    // Każdy stan powinien mieć shared_from_this
}


TransitionEffect::~TransitionEffect() {
    if (m_quadric) {
        gluDeleteQuadric(m_quadric);
        m_quadric = 0;
    }
}

TransitionEffectPtr TransitionEffect::New(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, double delay_before, double delay_after) {
    return TransitionEffectPtr(new TransitionEffect(from_state, to_state, effect_type, duration, delay_before, delay_after));
}

TransitionEffectPtr TransitionEffect::NewFadeIn(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay_before, double delay_after) {
    return TransitionEffectPtr(new TransitionEffect(from_state, to_state, TransitionEffectType::FadeIn, duration, delay_before, delay_after));
}

TransitionEffectPtr TransitionEffect::NewFadeOut(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay_before, double delay_after) {
    return TransitionEffectPtr(new TransitionEffect(from_state, to_state, TransitionEffectType::FadeOut, duration, delay_before, delay_after));
}

TransitionEffectPtr TransitionEffect::NewPinWheelOut(AppStatePtr from_state, AppStatePtr to_state, double duration, unsigned int blades, double delay_before, double delay_after) {
    return TransitionEffectPtr(new TransitionEffect(from_state, to_state, TransitionEffectType::PinWheelOut, duration, blades, delay_before, delay_after));
}

void TransitionEffect::Start() {
    m_timer = 0;
    if (m_effect_type==TransitionEffectType::FadeIn) {
        m_fade_alpha = 1;   // dużo czarnego i będzie coraz mniej
    } else if (m_effect_type==TransitionEffectType::FadeOut) {
        m_fade_alpha = 0;   // mało czarnego i będzie coraz więcej
    } else if (m_effect_type==TransitionEffectType::PinWheelOut) {
        if (m_quadric) {
            gluDeleteQuadric(m_quadric);
        }
        m_quadric = gluNewQuadric();
        m_sweep_angle = 0.0;
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
    } else if (m_effect_type==TransitionEffectType::PinWheelOut) {
        if (m_from_state) {
            m_from_state->SetClearBeforeDraw(false)->SetSwapAfterDraw(false);
            m_from_state->Draw();
        }
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glColor3f(1, 0, 0);
        glPushMatrix();
        {
            const double inner_radius = 0;
            const double outer_radius = .5 * 2;   // .5 * coś_większego_od_sqrt(2)
            glTranslated(.5, .5, 0);
            assert(m_blades_count>0 && "Niepoprawna wartosc parametru dla efektu PinWheelOut");
            for (unsigned i=0; i<m_blades_count; ++i) {
                gluPartialDisk(m_quadric, inner_radius, outer_radius, 20, 3, (i*360.0)/m_blades_count, m_sweep_angle);
            }
        }
        glPopMatrix();
        glPopAttrib();
    } else {
        assert(false && "Draw: Nieznany typ efektu");
    }

    Engine& engine = Engine::Get();
    engine.GetRenderer()->DrawQuad(0,0,1,1, 0,0,0, m_fade_alpha);

    SDL_GL_SwapBuffers();
}

bool TransitionEffect::Update(double dt) {
    // jeżeli efekt się zakończył to nic nie robimy
    if (IsDone() || (m_timer >= m_delay_before + m_duration + m_delay_after)) {
        SetDone(true);
        return IsDone();
    }
    m_timer += dt;

    // jeżeli nie upłynął jeszcze m_delay_before
    if (m_timer <= m_delay_before) {
    }

    // jeżeli upłynął czas trwania efektu
    if (m_delay_before + m_duration < m_timer) {
        if (m_effect_type==TransitionEffectType::PinWheelOut) {
            m_sweep_angle = 360.0;
        }
    }

    // efekt jest aktywny - upłynął czas delay_before, ale jeszcze nie jest w fazie delay_after
    if (m_delay_before <= m_timer && m_timer <= m_delay_before + m_duration) {
        if (m_effect_type==TransitionEffectType::FadeIn) {
            m_fade_alpha -= 1 * dt/m_duration;
        } else if (m_effect_type==TransitionEffectType::FadeOut) {
            m_fade_alpha += 1 * dt/m_duration;
        } else if (m_effect_type==TransitionEffectType::PinWheelOut) {
            m_sweep_angle += (360.0/m_blades_count) * dt/m_duration;
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
