#ifndef MAGAME_APP_STATE_H_INCLUDED
#define MAGAME_APP_STATE_H_INCLUDED
#include "StdAfx.h"


class AppState {
public:
    explicit AppState() 
        : m_is_done(false) {
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

private:
    bool m_is_done;                                 // czy stan się zakończył (i należy przejść do następnego)
};

class AppState;
typedef boost::shared_ptr<AppState> AppStatePtr;

#endif /* MAGAME_APP_STATE_H_INCLUDED */
