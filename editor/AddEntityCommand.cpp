#include "StdAfx.h"

#include "Editor.h"
#include "EditorCommand.h"
#include "game/entity/EntityFactory.h"

void AddEntityCommand::Execute(Editor* editor) {
    m_entity = m_entity_factory->CreateEntity(m_entity_data);
    editor->m_entities_to_create.push_back(m_entity_data);
    editor->m_entities.push_back(m_entity);
}

void AddEntityCommand::Undo(Editor* editor) {
    editor->m_entities_to_create.remove(m_entity_data);
    boost::remove_erase(editor->m_entities, m_entity);
}

bool AddEntityCommand::IsReady() const {
    return true;
}
