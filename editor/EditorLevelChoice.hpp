#ifndef MAGAME_EDITOR_LEVEL_CHOICE_HPP_INCLUDED
#define MAGAME_EDITOR_LEVEL_CHOICE_HPP_INCLUDED
#include "StdAfx.h"

#include "appstates/AppState.h"
#include "gui/ScrollBox.hpp"

class EditorLevelChoice;
typedef boost::shared_ptr<EditorLevelChoice> EditorLevelChoicePtr;

class EditorLevelChoice : public AppState
{
public:
    explicit EditorLevelChoice();

    virtual void Init();
    virtual void Start();

    virtual void Draw() const;
    virtual bool Update(double dt);
    virtual void ProcessEvents(SDL_Event const& event);

    virtual AppStatePtr NextAppState() const { return m_next_app_state; }
    void SetNextAppState(AppStatePtr state) { m_next_app_state = state; }

private:
    void EditLevel(std::string level_name);

    AppStatePtr m_next_app_state;
    boost::shared_ptr<gui::ScrollBox> m_level_list;
    std::deque<std::string> m_levels;
};

#endif // MAGAME_EDITOR_LEVEL_CHOICE_HPP_INCLUDED
