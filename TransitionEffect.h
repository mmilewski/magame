#ifndef __FADE_EFFECT_H_INCLUDED__
#define __FADE_EFFECT_H_INCLUDED__
#include "AppState.h"
#include "Types.h"


class TransitionEffect;
typedef boost::shared_ptr<TransitionEffect> TransitionEffectPtr;

// zbior efektow:
// http://service.real.com/help/library/guides/ProductionGuide/prodguide/htmfiles/transit.htm#43424

class TransitionEffect;
class tefFluent;
typedef boost::shared_ptr<TransitionEffect> tefPtr;
typedef boost::shared_ptr<tefFluent> tefFluentPtr;


class TransitionEffect : public AppState,
                         boost::noncopyable {
    TransitionEffectType::Type m_effect_type;     // typ efektu
    AppStatePtr m_from_state;             // stan, z którego robimy przejście
    AppStatePtr m_to_state;               // stan, do którego robimy przejœcie. Zostanie zwrócony po zakończeniu efektu
    double m_duration;                    // czas trwania efektu (nie wliczając delay_before oraz delay_after)
    double m_delay_before;                // opóźnienie od rozpoczęcia efektu
    double m_delay_after;                 // opóźnienie po zakończeniu efektu
    double m_start_fade_alpha;            // przezroczystość na początku efektu
    double m_end_fade_alpha;              // przezroczystość na końcu efektu
    int m_blades_count;                   // liczba ramion wiatraczka. Powinna być większa od 0
    double m_rot_angle;                   // łączny kąt o jaki będzie obrócony wiatraczek

    // pola dotyczące aktualnego stanu
    double m_current_fade_alpha;          // aktualna przezroczystość rysowanego czworokąta
    GLUquadricObj* m_quadric;             // obiekt do wyświetlania dysków/sześcianów/sfer/...
    double m_sweep_angle;                 // kąt wypełnienia dysku - 360 oznacza pełen dysk, a 0 - pusty
    double m_current_rot_angle;           // aktualny kąt obrotu wiatraczka
    double m_timer;                       // czas trwania stanu (pozwala okreœliæ czy już minał czas delay czy nie)

public:
    TransitionEffect(TransitionEffectType::Type effect_type)
        : m_effect_type(effect_type),
          m_from_state(),
          m_to_state(),
          m_duration(1),
          m_delay_before(0),
          m_delay_after(0),
          m_start_fade_alpha(0),
          m_end_fade_alpha(0),
          m_blades_count(0),
          m_rot_angle(0),
          m_current_fade_alpha(0),
          m_quadric(0),
          m_sweep_angle(0),
          m_current_rot_angle(0),
          m_timer(0) {
    }

    TransitionEffect(AppStatePtr from_state, AppStatePtr to_state, double duration)
        : m_effect_type(TransitionEffectType::UNKNOWN),
          m_from_state(from_state),
          m_to_state(to_state),
          m_duration(duration),
          m_delay_before(0),
          m_delay_after(0),
          m_start_fade_alpha(0),
          m_end_fade_alpha(0),
          m_blades_count(0),
          m_rot_angle(0),
          m_current_fade_alpha(0),
          m_quadric(0),
          m_sweep_angle(0),
          m_current_rot_angle(0),
          m_timer(0) {
    }
    
    ~TransitionEffect();

    static tefFluent Prepare(TransitionEffectType::Type effect_type);
    static tefFluent PrepareFadeIn(AppStatePtr next_state=AppStatePtr());
    static tefFluent PrepareFadeOut(AppStatePtr from_state=AppStatePtr());
    static tefFluent PreparePinWheelOut();

    void Start();
    void Init();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);
    AppStatePtr NextAppState() const;

public:
    void SetStates(AppStatePtr from, AppStatePtr to) { SetFromState(from); SetToState(to); }
    void SetFromState(AppStatePtr from) { m_from_state = from; }
    void SetToState(AppStatePtr to) { m_to_state = to; }
    void SetDuration(double duration) { m_duration = duration; }
    void SetDelay(double before, double after) {
        m_delay_before = before;
        m_delay_after = after;
    }
    void SetEffectType(TransitionEffectType::Type type) { m_effect_type = type; }
    void SetFadeAlpha(double start, double end) { 
        m_start_fade_alpha = start; 
        m_end_fade_alpha = end; 
    }
    void SetBladesCount(int count) { m_blades_count = count; }
    void SetRotation(double angle_in_degrees) { m_rot_angle = angle_in_degrees; }
};


class tefFluent {
    tefPtr m_instance;
public:
    tefFluent(TransitionEffectType::Type effect_type)
        : m_instance(new TransitionEffect(effect_type)) {
    }

    tefFluent(AppStatePtr from_state, AppStatePtr to_state, double duration)
        : m_instance(new TransitionEffect(from_state, to_state, duration)) {
    }

    tefFluent& from(AppStatePtr from_state) {
        m_instance->SetFromState(from_state);
        return *this;
    }

    tefFluent& to(AppStatePtr to_state) {
        m_instance->SetToState(to_state);
        return *this;
    }

    tefFluent& states(AppStatePtr from, AppStatePtr to) {
        m_instance->SetFromState(from);
        m_instance->SetToState(to);
        return *this;
    }

    tefFluent& duration(double duration_in_seconds) {
        m_instance->SetDuration(duration_in_seconds);
        return *this;
    }

    tefFluent& delay(double before, double after) {
        m_instance->SetDelay(before, after);
        return *this;
    }

    tefFluent& type(TransitionEffectType::Type effect_type) {
        m_instance->SetEffectType(effect_type);
        return *this;
    }

    tefFluent& fade_alpha(double start, double end) {
        m_instance->SetFadeAlpha(start, end);
        return *this;
    }

    tefFluent& blades(int count) {
        m_instance->SetBladesCount(count);
        return *this;
    }

    tefFluent& rotation(double angle) {
        m_instance->SetRotation(angle);
        return *this;
    }

    tefPtr Build() {
        return m_instance;
    }
};

#endif  // this file
