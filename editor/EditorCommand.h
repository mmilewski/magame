#ifndef __EDITORCOMMAND_H_INCLUDED__
#define	__EDITORCOMMAND_H_INCLUDED__

#include "../StdAfx.h"
#include "../BasicMathTypes.h"
#include "../Types.h"

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
        m_beg[0] = std::min(start.X(), end.X());
        m_end[0] = std::max(start.X(), end.X());
        m_beg[1] = std::min(start.Y(), end.Y());
        m_end[1] = std::max(start.Y(), end.Y());
    }

    virtual void Execute(Editor* editor);
    virtual void Undo(Editor* editor);
    virtual bool IsReady() const;

private:
    bool m_is_ready;         // Czy polecenie jest gotowe do wykonania
    Position m_beg, m_end;   // Początek i koniec zaznaczenia. Wsp.świata
    std::vector<FT::FieldType> m_saved_fields;  // zapisane pola planszy
};

#endif
