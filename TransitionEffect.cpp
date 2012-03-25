#include "StdAfx.h"

#include "TransitionEffect.h"
#include "Engine.h"

tefFluent TransitionEffect::Prepare(TransitionEffectType::Type effect_type) {
    if (effect_type==TransitionEffectType::FadeIn) {
        return tefFluent(effect_type).fade_alpha(1,0);
    } else if (effect_type==TransitionEffectType::FadeOut) {
        return tefFluent(effect_type).fade_alpha(0,1);
    } else if (effect_type==TransitionEffectType::PinWheelOut) {
        return tefFluent(effect_type).fade_alpha(0,1).blades(1);
    } else {
        return tefFluent(effect_type);
    }
}

tefFluent TransitionEffect::PrepareFadeIn(AppStatePtr next_state) {
    return TransitionEffect::Prepare(TransitionEffectType::FadeIn).to(next_state);
}

tefFluent TransitionEffect::PrepareFadeOut(AppStatePtr from_state) {
    return TransitionEffect::Prepare(TransitionEffectType::FadeOut).from(from_state);
}

tefFluent TransitionEffect::PreparePinWheelOut() {
    return TransitionEffect::Prepare(TransitionEffectType::PinWheelOut);
}


TransitionEffect::~TransitionEffect() {
    if (m_quadric) {
        gluDeleteQuadric(m_quadric);
        m_quadric = 0;
    }
}

void TransitionEffect::Start() {
    m_timer = 0;
    m_current_fade_alpha = m_start_fade_alpha;
    if (m_effect_type==TransitionEffectType::FadeIn) {
        //m_current_fade_alpha = 1;   // dużo czarnego i będzie coraz mniej
    } else if (m_effect_type==TransitionEffectType::FadeOut) {
        //m_current_fade_alpha = 0;   // mało czarnego i będzie coraz więcej
    } else if (m_effect_type==TransitionEffectType::PinWheelOut) {
        //m_current_fade_alpha = 0;
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

    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }

    if (m_effect_type==TransitionEffectType::FadeIn) {
        if (m_to_state) {
            m_to_state->SetClearBeforeDraw(false)->SetSwapAfterDraw(false);
            m_to_state->Draw();
        }
        Engine::Get().GetRenderer()->DrawQuad(0,0,1,1, 0,0,0, m_current_fade_alpha);
    } else if (m_effect_type==TransitionEffectType::FadeOut) {
        if (m_from_state) {
            m_from_state->SetClearBeforeDraw(false)->SetSwapAfterDraw(false);
            m_from_state->Draw();
        }
        Engine::Get().GetRenderer()->DrawQuad(0,0,1,1, 0,0,0, m_current_fade_alpha);
    } else if (m_effect_type==TransitionEffectType::PinWheelOut) {
        if (m_from_state) {
            m_from_state->SetClearBeforeDraw(false)->SetSwapAfterDraw(false);
            m_from_state->Draw();
        }
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4d(0, 0, 0, m_current_fade_alpha);
        glPushMatrix();
        {
            const double inner_radius = 0;
            const double outer_radius = .5 * 2;   // .5 * coś_większego_od_sqrt(2)
            glTranslated(.5, .5, 0);
            glRotated(m_current_rot_angle, 0, 0, 1);
            assert(m_blades_count>0 && "Niepoprawna wartosc parametru dla efektu PinWheelOut");
            for (int i=0; i<m_blades_count; ++i) {
                gluPartialDisk(m_quadric, inner_radius, outer_radius, 20, 3, (i*360.0)/m_blades_count, m_sweep_angle);
            }
        }
        glPopMatrix();
        glPopAttrib();
    } else {
        assert(false && "Draw: Nieznany typ efektu");
    }


    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
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
        m_current_fade_alpha += (m_end_fade_alpha-m_start_fade_alpha) * dt/m_duration;
        if (m_effect_type==TransitionEffectType::FadeIn) {
            //m_current_fade_alpha -= 1 * dt/m_duration;
        } else if (m_effect_type==TransitionEffectType::FadeOut) {
            //m_current_fade_alpha += 1 * dt/m_duration;
        } else if (m_effect_type==TransitionEffectType::PinWheelOut) {
            m_sweep_angle += (360.0/m_blades_count) * dt/m_duration;
            //m_current_fade_alpha += 1 * dt/m_duration;
            m_current_rot_angle += m_rot_angle * dt/m_duration;
        } else {
            assert(false && "Update: Nieznany typ efektu");
        }
    }
    return !IsDone();
}

void TransitionEffect::ProcessEvents(const SDL_Event& /* event */) {
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
