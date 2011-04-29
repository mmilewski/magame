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

    // Zwraca true jeśli zdarzenie przesunięcia myszy zostało obsłużone
    bool OnMouseMove(double x, double y);
    // Zwraca true jeśli zdarzenie naciśnięcia klawisza myszy zostało obsłużone
    bool OnMouseDown(Uint8 button, double x, double y);

    BrushPtr GetActiveBrush() const {
        return m_active_brush;
    }
    MultiBrushPtr GetActiveMultiBrush() const {
        return m_active_multibrush;
    }

private:
    typedef std::vector<BrushButtonPtr> BrushButtonContrainer;
    BrushButtonContrainer m_buttons;
    BrushButtonPtr m_hovered_button;
    BrushPtr m_active_brush;

    typedef std::vector<MultiBrushButtonPtr> MultiBrushButtonContrainer;
    MultiBrushButtonContrainer m_multibrush_buttons;
    MultiBrushButtonPtr m_hovered_multibrush_button;
    MultiBrushPtr m_active_multibrush;
};

#endif
