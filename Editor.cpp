#include "Engine.h"
#include "Text.h"
#include "Utils.h"
#include "Editor.h"

void Editor::Start() {
    m_gui->Start();
}

void Editor::Init() {
    m_level_view.StoreSprite(FT::EndOfLevel,       Sprite::GetByName("end_of_level"));
    m_level_view.StoreSprite(FT::PlatformLeftEnd,  Sprite::GetByName("platform_left"));
    m_level_view.StoreSprite(FT::PlatformMidPart,  Sprite::GetByName("platform_mid"));
    m_level_view.StoreSprite(FT::PlatformRightEnd, Sprite::GetByName("platform_right"));

    m_gui->Init();
}

void Editor::Draw() {
    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }
    const double offset = m_viewer_offset_x;

    // // -------------------- DEBUG ONLY --------------------
    // Text(0.04, 0.04).DrawText(IntToStr(int(offset))+"_"+IntToStr(int(offset*100)%100), 0.1, 0.2);
    // std::stringstream ss; ss << m_pointer_x << "_" << m_pointer_y;
    // Text(0.04, 0.04).DrawText(ss.str(), 0.1, 0.1);
    // // --------------------------------------------------

    glPushMatrix();
    {
        double viewer_x = -(offset * Engine::Get().GetRenderer()->GetTileWidth() - 0.45);
        glTranslated(viewer_x, 0, 0);
        m_level_view.SetLevel(m_level, offset);
        m_level_view.DrawHighlighted(offset, m_pointer_x, BottomUp(m_pointer_y));
    }
    glPopMatrix();

    // narysuj GUI -- wyłącz test głębokości, żeby zawsze było na wierzchu
    glPushAttrib(GL_DEPTH_TEST);
    {
        glDisable(GL_DEPTH_TEST);
        m_brush = m_gui->GetActiveBrush();
        if (m_brush) {
            const double tile_width = Engine::Get().GetRenderer()->GetTileWidth();
            const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();
            const double x = static_cast<int>(m_pointer_x) * tile_width;
            const double y = static_cast<int>(m_pointer_y) * tile_width;
            m_brush->GetSprite()->DrawCurrentFrame(x, y, tile_width, tile_height);
        }
        m_gui->Draw();
    }
    glPopAttrib();
    
    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}

bool Editor::Update(double dt) {
    if (m_keys_down[SDLK_ESCAPE]) {
        SetDone(true);
    } else if (m_keys_down[SDLK_LEFT]) {
        m_viewer_offset_x -= dt * 28.19;
    } else if (m_keys_down[SDLK_RIGHT]) {
        m_viewer_offset_x += dt * 28.24;
    }

    // upewnij się że edytor nie zagląda poza lewą krawędź planszy.
    // Znajdują się tam ujemne wartości na osi odciętych, co może
    // powodować błędy w obliczeniach.
    const double tiles_in_row = 1.0/Engine::Get().GetRenderer()->GetTileWidth();
    m_viewer_offset_x = std::max(m_viewer_offset_x, tiles_in_row/2-1);

    // aktualizacja GUI
    m_gui->Update(dt);

    return !IsDone();
}

void Editor::ProcessEvents(const SDL_Event& event) {
    if (IsDone()) {
        return;
    }

    if (event.type == SDL_QUIT) {
        SetDone();
    } else if (event.type == SDL_KEYDOWN) {
        if (m_gui->OnKeyDown(event.key.keysym.sym)==false) {
            m_keys_down[event.key.keysym.sym] = true;
        }
    } else if (event.type == SDL_KEYUP) {
        if (m_gui->OnKeyUp(event.key.keysym.sym)==false) {
            m_keys_down[event.key.keysym.sym] = false;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        m_pointer_window_x =       event.motion.x / static_cast<double>(Engine::Get().GetWindow()->GetWidth());
        m_pointer_window_y = 1.0 - event.motion.y / static_cast<double>(Engine::Get().GetWindow()->GetHeight());
        if (m_gui->OnMouseMove(m_pointer_window_x, m_pointer_window_y)) {
            m_pointer_x = m_pointer_y = 1000;   // move pointer away
        } else {
            m_pointer_x = MapWindowCoordToWorldX(m_pointer_window_x);
            m_pointer_y = MapWindowCoordToWorldY(m_pointer_window_y);
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        m_pointer_window_x =       event.motion.x / static_cast<double>(Engine::Get().GetWindow()->GetWidth());
        m_pointer_window_y = 1.0 - event.motion.y / static_cast<double>(Engine::Get().GetWindow()->GetHeight());
        if (m_gui->OnMouseDown(event.button.button, m_pointer_window_x, m_pointer_window_y)) {
        } else {
            m_pointer_x = MapWindowCoordToWorldX(m_pointer_window_x);
            m_pointer_y = MapWindowCoordToWorldY(m_pointer_window_y);
            BrushPtr b = m_gui->GetActiveBrush();
            if (b) {
                SetFieldAt(m_pointer_x, m_pointer_y, b->GetFieldType());
            } else {
                ClearFieldAt(m_pointer_x, m_pointer_y);
            }
        }
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

void Editor::ClearFieldAt(double x, double y) {
    SetFieldAt(x, y, FT::None);
}

void Editor::SetFieldAt(double x, double y, FT::FieldType ft) {
    m_level->SetField(static_cast<size_t>(x), static_cast<size_t>(TopDown(y)), ft);
}

FT::FieldType Editor::GetFieldAt(double x, double y) const {
    return m_level->Field(static_cast<size_t>(x), static_cast<size_t>(TopDown(y)));
}
