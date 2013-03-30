#ifndef MAGAME_EFFECTS_SHOW_H_INCLUDED
#define MAGAME_EFFECTS_SHOW_H_INCLUDED

#include "video/Rgba.h"

namespace demo {

class Show : public AppState, public boost::enable_shared_from_this<Show> {
private:
    std::list<TransitionEffectPtr> m_effects;
    AppStatePtr m_next;
    bool m_filled;
    mutable bool m_updated;

public:
    Show()
        : m_effects(),
          m_next(),
          m_filled(false),
          m_updated(false) {
    }

    void Init() {
        if (m_filled) {
            return ;
        }
        m_filled = true;
#define REPEAT(times) for (int i=1; i<=times; ++i)

        TransitionEffectPtr t1, t2;
        t1 = TransitionEffect::PrepareFadeIn().states(shared_from_this(),shared_from_this()).duration(3).Build();
        t2 = TransitionEffect::PrepareFadeOut().states(shared_from_this(),t1).duration(2).Build();
        m_effects.push_back(t2);
        REPEAT(2) {
            t1 = TransitionEffect::PrepareFadeIn().states(shared_from_this(),shared_from_this()).duration(1).Build();
            t2 = TransitionEffect::PrepareFadeOut().states(shared_from_this(),t1).duration(1).Build();
            m_effects.push_back(t2);
        }
        REPEAT(5) {
            m_effects.push_back(TransitionEffect::PrepareFadeOut().states(shared_from_this(),shared_from_this()).duration(.5).Build());
            m_effects.push_back(TransitionEffect::PrepareFadeOut().states(shared_from_this(),shared_from_this()).duration(.25).Build());
        }
        REPEAT(6) {
            m_effects.push_back(TransitionEffect::PrepareFadeOut().states(shared_from_this(),shared_from_this()).duration(.125).Build());
        }
        REPEAT(12) {
            m_effects.push_back(TransitionEffect::PrepareFadeOut().states(shared_from_this(),shared_from_this()).duration(.08).Build());
        }
        m_effects.push_back(TransitionEffect::PrepareFadeOut().states(shared_from_this(),shared_from_this()).duration(3).Build());
        REPEAT(3) {
            m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(i).Build());
            m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(i).fade_alpha(1,1).Build());
            m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(i).fade_alpha(.5,1).rotation(i*20).Build());
        }
        m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(5).Build());
        m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(10).Build());
        m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(20).Build());
        REPEAT(4) {
            m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(5).fade_alpha(1,0).rotation(25*i*i+i*10).Build());
        }
        REPEAT(4) {
            m_effects.push_back(TransitionEffect::PreparePinWheelOut().states(shared_from_this(),shared_from_this()).duration(2).blades(i).rotation(-(25*i*i+i*10)).Build());
        }
#undef REPEAT
    }

    void Start() {
    }

    void Draw() const {
        if (m_updated) {
            m_updated = false;
            return;
        }

        Engine::Get().GetRenderer()->DrawQuad( 0, 0,  1,  1, Rgba(1,1,0)); // yellow (na caly ekran)
        Engine::Get().GetRenderer()->DrawQuad(.3,.4, .8, .7, Rgba(1,0,0)); // red
        Engine::Get().GetRenderer()->DrawQuad(.4,.5, .5, .4, Rgba(0,0,1)); // blue
        Engine::Get().GetRenderer()->DrawQuad(.6,.5, .7, .4, Rgba(0,0,1)); // blue
    }

    bool Update(double /* dt */) {
        m_updated = true;
        if (false==m_effects.empty()) {
            TransitionEffectPtr effect = m_effects.front();
            m_effects.pop_front();
            m_next = effect;
            SetDone(true);
        }
        else {
            m_next.reset(new MainMenu);
        }        
        return !IsDone();
    }

    boost::shared_ptr<AppState> NextAppState() const { return m_next; }

    void ProcessEvents(const SDL_Event& /* event */) {
    }
};


} // namespace demo
#endif // MAGAME_EFFECTS_SHOW_H_INCLUDED
