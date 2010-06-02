#ifndef __FADE_EFFECT_H_INCLUDED__
#define __FADE_EFFECT_H_INCLUDED__
#include "AppState.h"
#include "Types.h"

class FadeEffect;
typedef boost::shared_ptr<FadeEffect> FadeEffectPtr;


class FadeEffect : public AppState {
public:
    explicit FadeEffect(AppStatePtr from_state, AppStatePtr to_state, FadeEffectType::Type effect_type, double duration, double delay=0.0);
    static FadeEffectPtr New(AppStatePtr from_state, AppStatePtr to_state, FadeEffectType::Type effect_type, double duration, double delay=0.0);
	static FadeEffectPtr NewFadeIn(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay=0.0);
	static FadeEffectPtr NewFadeOut(AppStatePtr from_state, AppStatePtr to_state, double duration, double delay=0.0);
    void Start();
    void Init();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);
    AppStatePtr NextAppState() const;

private:
    double m_delay;                       // opóźnienie od rozpoczêcia efektu
    double m_duration;                    // czas trwania efektu (nie wliczając delay)
    AppStatePtr m_from_state;             // stan, z którego robimy przejście
    AppStatePtr m_to_state;               // stan, do którego robimy przejœcie. Zostanie zwrócony po zakończeniu efektu
    FadeEffectType::Type m_effect_type;   // typ efektu
    double m_alpha;                       // aktualna przezroczystoœæ rysowanego czworokąta

    double m_timer;                       // czas trwania stanu (pozwala okreœliæ czy już minał czas delay czy nie)
};

#endif  // this file
