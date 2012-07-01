#ifndef MAGAME_APP_STATE_H_INCLUDED
#define MAGAME_APP_STATE_H_INCLUDED
#include "StdAfx.h"


class AppState {
public:
    explicit AppState() 
        : m_is_done(false),
          m_clear_before_draw(true),
          m_swap_after_draw(true) {
    }
    
    virtual ~AppState() {}
    
    virtual void Init() = 0;
    virtual void Start() = 0;

    virtual void Draw() const = 0;

    /**
     * @brief Update
     * @param dt Delta time to update state's logic (in seconds).
     * @return True iff state is done. Next state will be taken from NextAppState.
     */
    virtual bool Update(double dt) = 0;
    virtual void ProcessEvents(const SDL_Event& event) = 0;

    virtual boost::shared_ptr<AppState> NextAppState() const = 0;

    bool IsDone() const {
        return m_is_done;
    }

    void SetDone(bool value=true) {
        m_is_done = value;
    }

    bool IsClearBeforeDraw() const { return m_clear_before_draw; }
    bool IsSwapAfterDraw() const { return m_swap_after_draw; }

    AppState* SetClearBeforeDraw(bool clear) {
        m_clear_before_draw = clear;
        return this;
    }

    AppState* SetSwapAfterDraw(bool swap) {
        m_swap_after_draw = swap;
        return this;
    }

private:
    bool m_is_done;                                 // czy stan się zakończył (i należy przejść do następnego)
    bool m_clear_before_draw;                       // czy przed rysowaniem stanu będzie czyszczenie ekranu
    bool m_swap_after_draw;                         // czy rysowanie zakończy się podmianą buforów
};

class AppState;
typedef boost::shared_ptr<AppState> AppStatePtr;

#endif /* MAGAME_APP_STATE_H_INCLUDED */
