#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__
#include "StdAfx.h"

#include "AppState.h"

class MainMenu : public AppState {
private:
    struct Sel {
        enum Selection {
            None,
            NewGame,
            HallOfFame,
            Quit
        };
    };

public:
    explicit MainMenu()
        : m_selection(Sel::NewGame) {
    }

    void Init();
    void Start();

    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<AppState> NextAppState() const;

private: 
    Sel::Selection m_selection;
    AppStatePtr m_next_app_state;
};

#endif /* __MAIN_MENU_H__ */
