#ifndef __EDITOR_H_INCLUDED__
#define __EDITOR_H_INCLUDED__
#include "StdAfx.h"

// #include "Utils.h"
// #include "Player.h"
#include "Game.h"
#include "Level.h"
#include "SpriteGrid.h"
#include "Gui.h"
#include "AppState.h"

class Editor;
typedef boost::shared_ptr<Editor> EditorPtr;

class Editor : public AppState, public boost::enable_shared_from_this<Editor> {
public:
    explicit Editor(LevelPtr level) 
        : m_next_app_state(),
          m_in_game(false),
          m_gui(new EditorGui),
          m_is_gui_visible(true),
          m_level(level),
          m_viewer_offset_x(0),                            // Update zadba o poprawną wartość
          m_pointer_x(0), m_pointer_y(0),
          m_pointer_window_x(0), m_pointer_window_y(0),
          m_keys_down(SDLK_LAST, false)                    // Wszystkie klawisze puszczone
        {
            // nop
            SetDone(false);
    }

    void Start();
    void Init();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    AppStatePtr NextAppState() const {
        return m_next_app_state;
    }

    std::string GetLevelName() const {
        if (m_level) {
            return m_level->GetName();
        }
        return "unknown";
    }

private:
    void DrawEntitiesPlayerAndLevel(double viewer_x);
    void DrawBrushAndGui(double viewer_x);

    // Przekształca współrzędny kursora (przestrzeń okna)
    // na współrzędne świata (przestrzeń świata)
    double MapWindowCoordToWorldX(double x) const;
    double MapWindowCoordToWorldY(double y) const;

    // Czyści pole pod wskazanymi współrzędnymi (przestrzeń świata).
    // y -- bottom-up
    void ClearFieldAt(double x, double y);

    // Ustawia pole na wskazany typ
    // y -- bottom-up
    void SetFieldAt(double x, double y, FT::FieldType ft);

    // Zwraca typ pola we wskazanych współrzędnych (przestrzeń świata).
    // y -- bottom-up
    FT::FieldType GetFieldAt(double x, double y) const;

    // pokazuje/ukrywa gui
    void ToggleGui() { m_is_gui_visible = !m_is_gui_visible; }
    bool IsGuiVisible() const { return m_is_gui_visible; }
    bool IsGuiHidden()  const { return !IsGuiVisible(); }

    // w jakim trybie jest rysowany pędzel -- plansza jest wyróównywana do siatki
    // a encje (np. jednostki) - nie
    bool InPaintingFieldMode()   const { return m_brush && m_brush->IsField(); }
    bool InPaintingEntityMode()  const { return m_brush && m_brush->IsEntity(); }
    bool InPaintingSpecialMode() const { return m_brush && m_brush->IsSpecial(); }
    BrushPtr GetBrush()          const { return m_brush; }

    // czy rysowany obiekt (pod pędzlem) powinien być przyciągane do siatki
    bool ShouldSnapToGrid()      const;
        
    // metody do przełączania między trybem gry a trybem edytora
    void SwitchToGame()     { m_in_game = true; }
    void SwitchToEditor()   { m_in_game = false; m_game.reset(); }
    bool IsInGame() const   { return m_in_game; }
    bool IsInEditor() const { return !m_in_game; }

private:
    // TopDown odbija współrzędną y w pionie. Niektóre elementy kodu umiejscawiają
    // y=0 na górze świata, a inne na dole. Edytor zawsze działa z osią OY skierowanę
    // w górę, więc jeżeli pewna funkcja foo wymaga odbitego argumentu, to należy
    // wywołać ją jako foo(TopDown(some_y_coord)), by zaznaczyć, że pamiętaliśmy o odbiciu
    double TopDown(double y)  const { return Engine::Get().GetRenderer()->GetVerticalTilesOnScreenCount() - y; }

    Editor* SetBrush(BrushPtr brush) { m_brush = brush; return this; }

    void ActionAtCoords(double x, double y);

private:
    AppStatePtr m_next_app_state;
    bool m_in_game;                     // czy włączona jest gra?
    GamePtr m_game;                     // instancja gry

    BrushPtr m_brush;                   // pędzel do rysowania
    EditorGuiPtr m_gui;                 // kontrolki do wybierania stawianych pól
    bool m_is_gui_visible;              // czy kontrolki są widoczne?

    LevelPtr m_level;
    SpriteGrid m_level_view;
    double m_viewer_offset_x;           // przesunięcie środka planszy (przestrzeń świata)

    double m_pointer_x;                 // położenie kursora (przestrzeń świata)
    double m_pointer_y;                 // bottom-up
    double m_pointer_window_x;          // położenie kursora (przestrzeń okna)
    double m_pointer_window_y;          // bottom-up

    LevelEntityData m_player_data;                    // informacje o graczu
    std::vector<EntityPtr> m_entities;                // jednostki (są tylko rysowane)
    std::list<LevelEntityData> m_entities_to_create;  // opisy jednostek do stworzenia

    std::vector<bool> m_keys_down;
};

#endif
