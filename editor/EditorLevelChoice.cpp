#include "StdAfx.h"
#include "EditorLevelChoice.hpp"
#include "MainMenu.h"

EditorLevelChoice::EditorLevelChoice()
{
}

void EditorLevelChoice::Init()
{
}

void EditorLevelChoice::Start()
{
}

void EditorLevelChoice::Draw()
{
}

bool EditorLevelChoice::Update(double /*dt*/)
{
    return !IsDone();
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
        } else if (key == SDLK_DOWN) {
        } else if (key == SDLK_RETURN) {
        }
    }
}

