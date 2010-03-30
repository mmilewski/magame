#ifndef __APP_STATE_H__
#define __APP_STATE_H__
#include "StdAfx.h"

#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>

class AppState {
public:
    explicit AppState() 
        : m_is_done(false) {
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
};

typedef boost::shared_ptr<AppState> AppStatePtr;

#endif /* __APP_STATE_H__ */
