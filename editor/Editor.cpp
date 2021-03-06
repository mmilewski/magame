#include "StdAfx.h"

#include "Engine.h"
#include "video/Text.h"
#include "game/entity/Entity.h"
#include "appstates/MainMenu.h"
#include "editor/Editor.h"
#include "EditorLevelChoice.hpp"

void Editor::Start() {
    m_gui->Start();
}

void Editor::Init() {
    m_level_view.StoreSprite(FT::PlatformTopLeft,        Sprite::GetByName("PlatformTopLeft"));
    m_level_view.StoreSprite(FT::PlatformLeft,           Sprite::GetByName("PlatformLeft"));
    m_level_view.StoreSprite(FT::PlatformMid,            Sprite::GetByName("PlatformMid"));
    m_level_view.StoreSprite(FT::PlatformTop,            Sprite::GetByName("PlatformTop"));
    m_level_view.StoreSprite(FT::PlatformLeftTopRight,   Sprite::GetByName("PlatformLeftTopRight"));
    m_level_view.StoreSprite(FT::PlatformLeftRight,      Sprite::GetByName("PlatformLeftRight"));
    m_level_view.StoreSprite(FT::PlatformTopRight,       Sprite::GetByName("PlatformTopRight"));
    m_level_view.StoreSprite(FT::PlatformRight,          Sprite::GetByName("PlatformRight"));

    m_level_view.StoreSprite(FT::EndOfLevel,             Sprite::GetByName("EndOfLevel"));

    m_level_view.StoreSprite(FT::NcPlatformTopLeft,      Sprite::GetByName("NcPlatformTopLeft"));
    m_level_view.StoreSprite(FT::NcPlatformLeft,         Sprite::GetByName("NcPlatformLeft"));
    m_level_view.StoreSprite(FT::NcPlatformMid,          Sprite::GetByName("NcPlatformMid"));
    m_level_view.StoreSprite(FT::NcPlatformTop,          Sprite::GetByName("NcPlatformTop"));
    m_level_view.StoreSprite(FT::NcPlatformLeftTopRight, Sprite::GetByName("NcPlatformLeftTopRight"));
    m_level_view.StoreSprite(FT::NcPlatformLeftRight,    Sprite::GetByName("NcPlatformLeftRight"));
    m_level_view.StoreSprite(FT::NcPlatformTopRight,     Sprite::GetByName("NcPlatformTopRight"));
    m_level_view.StoreSprite(FT::NcPlatformRight,        Sprite::GetByName("NcPlatformRight"));

    m_gui->Init();

    for(const LevelEntityData& data : m_level->GetAllEntitiesToCreate()) {
        m_entities_to_create.push_back(data);
        m_entities.push_back(m_entity_factory->CreateEntity(data));
    }
    m_player_data = m_level->GetPlayerData();
}

bool Editor::Update(double dt) {
    if (IsInGame()) {
        m_game->Update(dt);
        if (m_game->IsDone()) {
            SwitchToEditor();
        }
        return !IsDone();
    }

    if (m_keys_down[SDLK_ESCAPE]) {
        m_next_app_state.reset(new EditorLevelChoice);
        SetDone(true);
    } else if (m_keys_down[SDLK_LEFT]) {
        m_viewer_offset_x -= dt * 28.19;
    } else if (m_keys_down[SDLK_RIGHT]) {
        m_viewer_offset_x += dt * 28.24;
    }

    // upewnij się że edytor nie zagląda poza lewą krawędź planszy. Znajdują się
    // tam ujemne wartości na osi odciętych, co może powodować błędy w obliczeniach
    const double tiles_in_row = 1.0/Engine::Get().GetRenderer()->GetTileWidth();
    m_viewer_offset_x = std::max(m_viewer_offset_x, tiles_in_row/2-1);

    // aktualizacja GUI
    m_gui->Update(dt);
    SetBrush(m_gui->GetActiveBrush());

    return !IsDone();
}

void Editor::Draw() const {
    if (IsInGame()) {
        m_game->Draw();
        return;
    }

    glPushAttrib(GL_COLOR_BUFFER_BIT);
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const double tile_width = Engine::Get().GetRenderer()->GetTileWidth();
        const double viewer_x   = -(m_viewer_offset_x * tile_width - 0.45);

        DrawEntitiesPlayerAndLevel(viewer_x);
        DrawBrushAndGui(viewer_x);
    }
    glPopAttrib();
}

void Editor::DrawEntitiesPlayerAndLevel(double viewer_x) const {
    const double tile_width  = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();

    glPushMatrix();
    {
        glTranslated(viewer_x, 0, 0);

        // jednostki i gracz
        std::for_each(m_entities.begin(), m_entities.end(), boost::bind(&Entity::Draw, _1));
        if (m_player_data.x >= 0) {
            const SpritePtr player_sprite = Sprite::GetByName("player_stop");
            const double player_x(m_player_data.x * tile_width), player_y(m_player_data.y * tile_height);
            player_sprite->DrawCurrentFrame(player_x, player_y, tile_width, tile_height);
        } else {
            // nie ma danych gracza, to znaczy, że nie został ustawiony. Dlatego
            // nie rysujemy jego sprite'a. Przy przejściu do podglądu gry zostanie
            // dodany na domyślnej pozycji.
        }

        // poziom
        double offset = m_viewer_offset_x;
        m_level_view.SetLevel(m_level, offset);
        m_level_view.Draw(offset);
    }
    glPopMatrix();
}

void Editor::DrawBrushAndGui(double viewer_x) const {
    const double tile_width  = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();

    glPushAttrib(GL_ENABLE_BIT);
    {
        // wyłącz test głębokości, żeby pędzel oraz gui były zawsze na wierzchu
        glDisable(GL_DEPTH_TEST);
        if (GetBrush()) {
            glPushMatrix();
            {
                glTranslated(viewer_x, 0, 0);
                Size size(tile_width, tile_height);
                Position position(m_pointer_x * tile_width, m_pointer_y * tile_height);
                // niektóre pola są wyrównywane do siatki, więc trzeba przyciąć współrzędne

                if (ShouldSnapToGrid()) {
                    position = Position(static_cast<int>(m_pointer_x) * tile_width,
                                        static_cast<int>(m_pointer_y) * tile_height);
                }
                Engine::Get().GetRenderer()->DrawQuad(position, position+size, Rgba(1,1,1,.4)); // podświetlenie
                GetBrush()->Draw(position, size);
            }
            glPopMatrix();
        }
        if (IsGuiVisible()) {
            m_gui->Draw();
        }
    }
    glPopAttrib();
}

bool Editor::ShouldSnapToGrid() const {
    if (BrushPtr brush = GetBrush()) {
        return brush->SnapsToGrid();
    }
    return false;
}

void Editor::StartAtCoords(double x, double y) {
    if (BrushPtr brush = GetBrush()) {
        brush->StartAt(x, y);
    }
}

void Editor::CursorMovedToCoords(double x, double y) {
    if (BrushPtr brush = GetBrush()) {
        brush->MoveTo(x, y);
    }
}

void Editor::ReleaseAtCoords(double x, double y) {
    if (BrushPtr brush = GetBrush()) {
        brush->FinishAt(x, y);
        RegisterAndExecuteCommand(brush->GetCommand());
    }
}

void Editor::RegisterAndExecuteCommand(EditorCommandPtr command) {
    if (command && command->IsReady()) {
        command->Execute(this);
        m_commands.push_back(command);
    }
}

void Editor::ProcessEvents(const SDL_Event& event) {
    if (IsDone()) {
        return;
    }

    if (event.type == SDL_KEYUP && event.key.keysym.sym==SDLK_0) {
        if (IsInGame()) {
            SwitchToEditor();
        } else {
            LevelPtr level(new Level(m_level, m_entities_to_create, m_player_data));
            level->ShrinkWidth();
            level->SaveEntitiesToFile("data/new.ents");
            level->SaveFieldsToFile("data/new.lvl");
            m_game.reset(new Game(level, PlayerPtr()));
            m_game->Init();
            m_game->Start();
            SwitchToGame();
        }
        return;
    }
    if (IsInGame()) {
        m_game->ProcessEvents(event);
        return;
    }

    double win_width = static_cast<double>(Engine::Get().GetWindow()->GetWidth());
    double win_height= static_cast<double>(Engine::Get().GetWindow()->GetHeight());
    if (event.type == SDL_QUIT) {
        SetDone();
    } else if (event.type == SDL_KEYDOWN) {
        if (IsGuiHidden() || m_gui->OnKeyDown(event.key.keysym.sym)==false) {
            m_keys_down[event.key.keysym.sym] = true;
        }
        if (event.key.keysym.sym == SDLK_BACKSPACE && m_commands.empty()==false) {
            m_commands.back()->Undo(this);
            m_commands.pop_back();
        }
    } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym==SDLK_1) {
            ToggleGui();
        } else if (IsGuiHidden() || m_gui->OnKeyUp(event.key.keysym.sym)==false) {
            m_keys_down[event.key.keysym.sym] = false;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        m_pointer_window_x =       event.motion.x / win_width;
        m_pointer_window_y = 1.0 - event.motion.y / win_height;
        if (IsGuiVisible() && m_gui->OnMouseMove(m_pointer_window_x, m_pointer_window_y)) {
            // przesuń kursor poza ekran (można to zrobić ładniej)
            m_pointer_x = m_pointer_y = 1000;
        } else {
            m_pointer_x = MapWindowCoordToWorldX(m_pointer_window_x);
            m_pointer_y = MapWindowCoordToWorldY(m_pointer_window_y);
            CursorMovedToCoords(m_pointer_x, m_pointer_y);
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        m_pointer_window_x =       event.motion.x / win_width;
        m_pointer_window_y = 1.0 - event.motion.y / win_height;
        if (IsGuiVisible() && m_gui->OnMouseDown(event.button.button, m_pointer_window_x, m_pointer_window_y)) {
        } else {
            m_pointer_x = MapWindowCoordToWorldX(m_pointer_window_x);
            m_pointer_y = MapWindowCoordToWorldY(m_pointer_window_y);
            StartAtCoords(m_pointer_x, m_pointer_y);
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        m_pointer_x = MapWindowCoordToWorldX(m_pointer_window_x);
        m_pointer_y = MapWindowCoordToWorldY(m_pointer_window_y);
        ReleaseAtCoords(m_pointer_x, m_pointer_y);
    }
}

double Editor::MapWindowCoordToWorldX(double x) const {
    const double tiles_in_row = 1.0/Engine::Get().GetRenderer()->GetTileWidth();
    double k = x*tiles_in_row + m_viewer_offset_x - tiles_in_row/2 + 1;
    return k;
}

double Editor::MapWindowCoordToWorldY(double y) const {
    const double th = Engine::Get().GetRenderer()->GetTileHeight();
    return y/th;
}

Position Editor::MapWindowCoordsToWorld(const Position& coords) const {
    return Position(MapWindowCoordToWorldX(coords.X()),
                    MapWindowCoordToWorldY(coords.Y()));
}

void Editor::ClearFieldAt(double x, double y) {
    SetFieldAt(x, y, FT::None);
}
void Editor::ClearFieldAt(const Position& pos) {
    ClearFieldAt(pos.X(), pos.Y());
}

void Editor::SetFieldAt(double x, double y, FT::FieldType ft) {
    m_level->EnsureWidth(static_cast<size_t>(x+1));
    m_level->SetField(static_cast<size_t>(x), static_cast<size_t>(TopDown(y)), ft);
}

void Editor::SetFieldAt(const Position& pos, FT::FieldType ft) {
    SetFieldAt(pos.X(), pos.Y(), ft);
}

FT::FieldType Editor::GetFieldAt(double x, double y) const {
    return m_level->Field(static_cast<size_t>(x), static_cast<size_t>(TopDown(y)));
}
FT::FieldType Editor::GetFieldAt(const Position& pos) const {
    return GetFieldAt(pos.X(), pos.Y());
}
