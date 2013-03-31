#include "StdAfx.h"

#include "Editor.h"
#include "EditorCommand.h"
#include "game/entity/EntityFactory.h"

void AddEntityCommand::Execute(Editor* editor) {
    assert(m_entity_type != ET::UNKNOWN);
    assert(m_entity_type != ET::COUNT);

    const std::string name = EntityTypeAsString(m_entity_type);
    m_entity_data = LevelEntityData(name, m_pos);
    m_entity = editor->GetEntityFactory()->CreateEntity(m_entity_data);
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
