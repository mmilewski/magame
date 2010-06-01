#ifndef __APP_STATE_H__
#define __APP_STATE_H__
#include "StdAfx.h"


class AppState {
public:
    explicit AppState() 
        : m_is_done(false),
          m_clear_before_draw(true),
          m_swap_after_draw(true) {
    }

    virtual void Init() = 0;
    virtual void Start() = 0;

    virtual void Draw() = 0;
    virtual bool Update(double dt) = 0;
    virtual void ProcessEvents(const SDL_Event& event) = 0;

    virtual boost::shared_ptr<AppState> NextAppState() const = 0;

    bool IsDone() const {
        return m_is_done;
    }

    void SetDone(bool value=true) {
        m_is_done = value;
    }

private:
    bool m_is_done;

// TODO: to nie powinno byæ publiczne
public:
    bool m_clear_before_draw;
    bool m_swap_after_draw;
};

typedef boost::shared_ptr<AppState> AppStatePtr;

#endif /* __APP_STATE_H__ */
