#ifndef __EDITORCOMMAND_H_INCLUDED__
#define	__EDITORCOMMAND_H_INCLUDED__

#include "../StdAfx.h"

class EditorCommand;
typedef boost::shared_ptr<EditorCommand> EditorCommandPtr;

class EditorCommand {
public:
    virtual ~EditorCommand() {}

public:
    virtual void Execute() = 0;
    virtual void Undo() = 0;
};


class PlatformEditorCommand;
typedef boost::shared_ptr<PlatformEditorCommand> PlatformEditorCommandPtr;

class PlatformEditorCommand : public EditorCommand {
public:
    explicit PlatformEditorCommand() : EditorCommand() {    
    }
    
    virtual void Execute() {
        std::cerr << "Executing PlatformEditorCommand" << std::endl;
    }

    virtual void Undo() {
        std::cerr << "Undoing PlatformEditorCommand" << std::endl;
    }

//    virtual void ActionAt(double x, double y) {
//        m_startx = x;
//        m_starty = y;
//    }
//
//    virtual void ReleaseAt(double x, double y) {
//        m_endx = x;
//        m_endy = y;
//    }
//
//    virtual void MoveAt(double x, double y) {
//
//    }
//
//private:
//    double m_startx, m_starty;
//    double m_endx, m_endy;
};

#endif
