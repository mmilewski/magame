#include "Editor.h"
#include "EditorCommand.h"

void SetFieldCommand::Execute(Editor* editor) {
    m_saved_field = editor->GetFieldAt(m_pos);
    editor->SetFieldAt(m_pos, m_field);
}

void SetFieldCommand::Undo(Editor* editor) {
    editor->SetFieldAt(m_pos, m_saved_field);
}

bool SetFieldCommand::IsReady() const {
    if (m_pos.X() < 0 || m_pos.Y() < 0) return false;
    if (m_field == FT::COUNT) return false;
    return true;
}
