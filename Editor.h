#ifndef __EDITOR_H_INCLUDED__
#define __EDITOR_H_INCLUDED__
#include "StdAfx.h"

// #include "Utils.h"
// #include "Player.h"
#include "Level.h"
#include "SpriteGrid.h"
// #include "Entity.h"
#include "Hud.h"
#include "AppState.h"

class Game;

class Editor : public AppState, public boost::enable_shared_from_this<Editor> {
public:
    explicit Editor(LevelPtr level) 
        : m_next_app_state(),
          m_hud(new EditorHud),
          m_level(level),
          m_viewer_offset_x(0),                            // Update zadba o poprawną wartość
          m_pointer_x(0), m_pointer_y(0),
          m_pointer_window_x(0), m_pointer_window_y(0),
          m_keys_down(SDLK_LAST, false)                    // Wszystkie klawisze puszczone
        {
            // nop
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
    // Przekształca współrzędny kursora (przestrzeń okna)
    // na współrzędne świata (przestrzeń świata)
    double MapWindowCoordToWorldX(double x) const;
    double MapWindowCoordToWorldY(double y) const;

    // Czyści pole pod wskazanymi współrzędnymi (przestrzeń świata).
    // y -- bottom-up.
    void ClearFieldAt(double x, double y);

    // Zwraca typ pola we wskazanych współrzędnych (przestrzeń świata).
    // y -- bottom-up
    FT::FieldType GetFieldAt(double x, double y) const;

    // BottomUp odbija współrzędną y w pionie. Niektóre elementy kodu umiejscawiają
    // y=0 na górze świata, a inne na dole. Zwraca y jako bottom-up
    // y -- top-down
    // TopDown działa odwrotnie
    double BottomUp(double y) const { return 1.0/Engine::Get().GetRenderer()->GetTileHeight() - y; }
    double TopDown(double y)  const { return 1.0/Engine::Get().GetRenderer()->GetTileHeight() - y; }


private:
    AppStatePtr m_next_app_state;

    HudPtr m_hud;                       // head up diplay

    LevelPtr m_level;
    SpriteGrid m_level_view;
    double m_viewer_offset_x;           // przesunięcie środka planszy (przestrzeń świata)

    double m_pointer_x;                 // położenie kursora (przestrzeń świata)
    double m_pointer_y;                 // bottom-up
    double m_pointer_window_x;          // położenie kursora (przestrzeń okna)
    double m_pointer_window_y;          // bottom-up

    // PlayerPtr m_player;
    // std::vector<EntityPtr> m_entities;                // jednostki
    // std::list<LevelEntityData> m_entities_to_create;  // opisy jednostek do stworzenia
    std::vector<bool> m_keys_down;
};

typedef boost::shared_ptr<Editor> EditorPtr;

#endif
