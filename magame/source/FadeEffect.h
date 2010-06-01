#ifndef __FADE_EFFECT_H_INCLUDED__
#define __FADE_EFFECT_H_INCLUDED__
#include "AppState.h"
#include "Types.h"


class FadeEffect : public AppState {
public:
    explicit FadeEffect(double delay, double duration, AppStatePtr from_state, AppStatePtr to_state, FadeEffectType::Type effect_type);
    void Start();
    void Init();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);
    AppStatePtr NextAppState() const;

private:
    double m_delay;                       // op�nienie od rozpocz�cia efektu
    double m_duration;                    // czas trwania efektu (nie wliczaj�c delay)
    AppStatePtr m_from_state;             // stan, z kt�rego robimy przej�cie
    AppStatePtr m_to_state;               // stan, do kt�rego robimy przej�cie. Zostanie zwr�cony po zako�czeniu efektu
    FadeEffectType::Type m_effect_type;   // typ efektu
    double m_alpha;                       // aktualna przezroczysto�� rysowanego czworok�ta

    double m_timer;                       // czas trwania stanu (pozwala okre�li� czy ju� mina� czas delay czy nie)
};

#endif  // this file
