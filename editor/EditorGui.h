#ifndef MAGAME_EDITORGUI_H_INCLUDED
#define MAGAME_EDITORGUI_H_INCLUDED
#include "StdAfx.h"

#include "../gui/AbstractGui.h"
#include "BrushButton.h"

class EditorGui;
typedef boost::shared_ptr<EditorGui> EditorGuiPtr;

class EditorGui : public gui::AbstractGui, public boost::enable_shared_from_this<EditorGui> {
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

private:
    typedef std::vector<BrushButtonPtr> BrushButtonContrainer;
    BrushButtonContrainer m_buttons;
    BrushButtonPtr m_hovered_button;
    BrushPtr m_active_brush;
};

#endif
