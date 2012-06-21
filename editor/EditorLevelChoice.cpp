#include "StdAfx.h"
#include "EditorLevelChoice.hpp"
#include "MainMenu.h"
#include "Engine.h"
#include "Level.h"
#include "editor/Editor.h"

EditorLevelChoice::EditorLevelChoice()
{
}

void EditorLevelChoice::Init()
{
    std::deque<std::string> levels = {"new game", "1", "2", "3", "4", "5"};
    m_level_list.reset(new gui::ScrollBox(Position(0.25,0.2), Size(.5, .6)));
    m_level_list->SetItems(levels);
}

void EditorLevelChoice::Start()
{
}

void EditorLevelChoice::Draw()
{
    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }

    m_level_list->Draw();

    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}

bool EditorLevelChoice::Update(double dt)
{
    m_level_list->Update(dt);
    return !IsDone();
}

void EditorLevelChoice::EditLevel(std::string level_name)
{
    LevelPtr level(new Level);
    level->LoadFromFile("data/" + level_name + ".lvl");
    level->LoadEntitiesFromFile("data/" + level_name + ".ents");
    AppStatePtr editorState(new Editor(level));
    SetNextAppState(editorState);
    SetDone();
}

void EditorLevelChoice::ProcessEvents(SDL_Event const & event)
{
    if (event.type == SDL_QUIT) {
        SetDone();
    }

    if (event.type == SDL_KEYDOWN) {
        SDLKey key = event.key.keysym.sym;
        if (key == SDLK_ESCAPE) {
            SetNextAppState(AppStatePtr(new MainMenu));
            SetDone();
        } else if (key == SDLK_LEFT) {
        } else if (key == SDLK_RIGHT) {
        } else if (key == SDLK_UP) {
            m_level_list->SelectPrev();
        } else if (key == SDLK_DOWN) {
            m_level_list->SelectNext();
        } else if (key == SDLK_RETURN && m_level_list->HasSelection()) {
            if (m_level_list->SelectedId() == 0) {
                /*
                 * TOOD: Before we open the editor, we should ask for level's name.
                 */
                std::string level_name = "new";
                EditLevel(level_name);
            } else {
                EditLevel(IntToStr(m_level_list->SelectedId()));
            }
        }
    }
}

