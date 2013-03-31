#include "StdAfx.h"

#include "Editor.h"
#include "EditorCommand.h"

SetPlayerCommand::SetPlayerCommand(Position pos)
    : m_new_pos(pos) {
}

void SetPlayerCommand::Execute(Editor* editor) {
    m_saved_data = editor->m_player_data;
    editor->m_player_data = LevelEntityData("player", m_new_pos);
}

void SetPlayerCommand::Undo(Editor* editor) {
    editor->m_player_data = m_saved_data;
}

bool SetPlayerCommand::IsReady() const {
    return m_new_pos.X() > 0 && m_new_pos.Y() > 0;
}

