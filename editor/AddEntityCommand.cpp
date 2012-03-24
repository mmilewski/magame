#include "Editor.h"
#include "EditorCommand.h"
#include "entity/EntityFactory.h"

void AddEntityCommand::Execute(Editor* editor) {
    m_entity = m_entity_factory->CreateEntity(m_entity_data);
    editor->m_entities_to_create.push_back(m_entity_data);
    editor->m_entities.push_back(m_entity);
}

void AddEntityCommand::Undo(Editor* editor) {
    editor->m_entities_to_create.remove(m_entity_data);
    editor->m_entities.erase(boost::remove(editor->m_entities, m_entity), editor->m_entities.end());
}

bool AddEntityCommand::IsReady() const {
    return true;
}
