#ifndef __EDITORGUI_H_INCLUDED__
#define __EDITORGUI_H_INCLUDED__

#include "../gui/Gui.h"
#include "BrushButton.h"

class EditorGui;
typedef boost::shared_ptr<EditorGui> EditorGuiPtr;

class EditorGui : public Gui, public boost::enable_shared_from_this<EditorGui> {
public:
    explicit EditorGui();
    void Start();
    void Init();
    void Draw();
    void Update(double dt);

    bool OnMouseMove(double x, double y);
    bool OnMouseDown(Uint8 button, double x, double y);

    BrushPtr GetActiveBrush() const {
        return m_active_brush;
    }

private:
    typedef std::vector<BrushButtonPtr> BrushButtonContrainer;
    BrushButtonContrainer m_widgets;
    BrushButtonPtr m_hovered_button;
    BrushPtr m_active_brush;
};

#endif
