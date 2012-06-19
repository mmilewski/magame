#ifndef MAGAME_EDITOR_LEVEL_CHOICE_HPP_INCLUDED
#define MAGAME_EDITOR_LEVEL_CHOICE_HPP_INCLUDED
#include "StdAfx.h"

#include "AppState.h"

class EditorLevelChoice;
typedef boost::shared_ptr<EditorLevelChoice> EditorLevelChoicePtr;

class EditorLevelChoice : public AppState
{
public:
    explicit EditorLevelChoice();

    virtual void Init();
    virtual void Start();

    virtual void Draw();
    virtual bool Update(double dt);
    virtual void ProcessEvents(SDL_Event const& event);

    virtual AppStatePtr NextAppState() const { return m_next_app_state; }
    void SetNextAppState(AppStatePtr state) { m_next_app_state = state; }
private:
    AppStatePtr m_next_app_state;
};

#endif // MAGAME_EDITOR_LEVEL_CHOICE_HPP_INCLUDED
