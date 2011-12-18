#include "Editor.h"
#include "EditorCommand.h"
#include "entity/EntityFactory.h"
#include <algorithm>  // remove

void AddEntityCommand::Execute(Editor* editor) {
    m_entity = EntityFactory().CreateEntity(m_entity_data);
    editor->m_entities_to_create.push_back(m_entity_data);
    editor->m_entities.push_back(m_entity);
}

void AddEntityCommand::Undo(Editor* editor) {
    editor->m_entities_to_create.remove(m_entity_data);
    editor->m_entities.erase(std::remove(editor->m_entities.begin(),
                                         editor->m_entities.end(),
                                         m_entity),
                             editor->m_entities.end());
}

bool AddEntityCommand::IsReady() const {
    return true;
}
