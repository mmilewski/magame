#ifndef __FADE_EFFECT_H_INCLUDED__
#define __FADE_EFFECT_H_INCLUDED__
#include "AppState.h"
#include "Types.h"

class TransitionEffect;
typedef boost::shared_ptr<TransitionEffect> TransitionEffectPtr;

// zbior efektow:
// http://service.real.com/help/library/guides/ProductionGuide/prodguide/htmfiles/transit.htm#43424

// TODO: 
//  - dodac obracanie do pinwheelout
//  - zrobic fluent interface


class TransitionEffect : public AppState {
public:
    explicit TransitionEffect(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, double delay_before=0.0, double delay_after=0.0);
    explicit TransitionEffect(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, unsigned int blades, double delay_before=0.0, double delay_after=0.0);
    static TransitionEffectPtr New(AppStatePtr from_state, AppStatePtr to_state, TransitionEffectType::Type effect_type, double duration, double delay_before=0.0, double delay_after=0.0);
    static TransitionEffectPtr NewFadeIn(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay_before=0.0, double delay_after=0.0);
    static TransitionEffectPtr NewFadeOut(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay_before=0.0, double delay_after=0.0);
    static TransitionEffectPtr NewPinWheelOut(AppStatePtr from_state, AppStatePtr to_state, double duration, unsigned int blades, double delay_before=0.0, double delay_after=0.0);
    
    ~TransitionEffect();

    void Start();
    void Init();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);
    AppStatePtr NextAppState() const;

private:
    double m_delay_before;                // opóźnienie od rozpoczęcia efektu
    double m_delay_after;                 // opóźnienie po zakończeniu efektu
    double m_duration;                    // czas trwania efektu (nie wliczając delay_before oraz delay_after)
    AppStatePtr m_from_state;             // stan, z którego robimy przejście
    AppStatePtr m_to_state;               // stan, do którego robimy przejœcie. Zostanie zwrócony po zakończeniu efektu
    TransitionEffectType::Type m_effect_type;     // typ efektu
    
    double m_fade_alpha;                  // aktualna przezroczystość rysowanego czworokąta
    
    GLUquadricObj* m_quadric;             // obiekt do wyświetlania dysków/sześcianów/sfer/...
    double m_sweep_angle;                 // kąt wypełnienia dysku - 360 oznacza pełen dysk, a 0 - pusty
    unsigned int m_blades_count;          // ilość ramion wiatraczka

    double m_timer;                       // czas trwania stanu (pozwala okreœliæ czy już minał czas delay czy nie)
};

#endif  // this file
