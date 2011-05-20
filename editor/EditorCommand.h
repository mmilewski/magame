#ifndef __EDITORCOMMAND_H_INCLUDED__
#define	__EDITORCOMMAND_H_INCLUDED__

#include "../StdAfx.h"
#include "../Entity.h"

class Editor;

class EditorCommand;
typedef boost::shared_ptr<EditorCommand> EditorCommandPtr;

class EditorCommand {
public:
    virtual ~EditorCommand() {}

public:
    virtual void Execute(Editor* editor) = 0;
    virtual void Undo(Editor* editor) = 0;

    // False, jeśli polecenie nie może zostać wykonane, bo np. przekazane
    // parametry nie pozwalają na wykonanie sensownej akcji (wtedy Undo też by
    // nie miało sensu).
    virtual bool IsReady() const = 0;
    bool IsNotReady() const { return !IsReady(); }
};


class PlatformEditorCommand;
typedef boost::shared_ptr<PlatformEditorCommand> PlatformEditorCommandPtr;

class PlatformEditorCommand : public EditorCommand {
public:
    explicit PlatformEditorCommand(const Position& start, const Position& end)
      : m_is_ready(false),
        m_beg(start), m_end(end) {
    }

    virtual void Execute(Editor* editor);
    virtual void Undo(Editor* editor);
    virtual bool IsReady() const;

private:
    bool m_is_ready;         // Czy polecenie jest gotowe do wykonania
    Position m_beg, m_end;   // Początek i koniec zaznaczenia. Wsp.świata
    std::vector<FT::FieldType> m_saved_fields;  // zapisane pola planszy
};


class SetFieldCommand;
typedef boost::shared_ptr<SetFieldCommand> SetFieldCommandPtr;

class SetFieldCommand : public EditorCommand {
public:
    explicit SetFieldCommand(const Position& pos, FT::FieldType field)
      : m_is_ready(false),
        m_pos(pos),
        m_field(field) {
    }

    explicit SetFieldCommand(size_t x, size_t y, FT::FieldType field)
      : m_is_ready(false),
        m_pos(Position(x, y)),
        m_field(field) {
    }

    virtual void Execute(Editor* editor);
    virtual void Undo(Editor* editor);
    virtual bool IsReady() const;

private:
    bool m_is_ready;         // Czy polecenie jest gotowe do wykonania
    Position m_pos;          //
    FT::FieldType m_field, m_saved_field;
};


class AddEntityCommand;
typedef boost::shared_ptr<AddEntityCommand> AddEntityCommandPtr;

class AddEntityCommand : public EditorCommand {
public:
    explicit AddEntityCommand(const LevelEntityData& entity_data)
      : m_entity_data(entity_data) {
    }

    virtual void Execute(Editor* editor);
    virtual void Undo(Editor* editor);
    virtual bool IsReady() const;

private:
    LevelEntityData m_entity_data;
    EntityPtr m_entity;
};

#endif
