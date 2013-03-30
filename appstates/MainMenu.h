#ifndef MAGAME_MAIN_MENU_H_INCLUDED
#define MAGAME_MAIN_MENU_H_INCLUDED
#include "StdAfx.h"

#include "AppState.h"

class MainMenu : public AppState, public boost::enable_shared_from_this<MainMenu> {
private:
    struct Sel {
        enum Selection {
            None,
            NewGame,
            HallOfFame,
            Editor,
            Quit
        };
    };

public:
    explicit MainMenu()
        : m_selection(Sel::NewGame),
          m_next_app_state() {
    }

    void Init();
    void Start();

    void Draw() const;
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<AppState> NextAppState() const;

private: 
    Sel::Selection m_selection;
    AppStatePtr m_next_app_state;
};

#endif /* MAGAME_MAIN_MENU_H_INCLUDED */
