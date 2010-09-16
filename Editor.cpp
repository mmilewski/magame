#include "Engine.h"
#include "Text.h"
#include "Utils.h"
#include "Entity.h"
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
    const double tile_width  = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();
    const double viewer_x = -(offset * tile_width - 0.45);

    // // -------------------- DEBUG ONLY --------------------
    // Text(0.04, 0.04).DrawText(IntToStr(int(offset))+"_"+IntToStr(int(offset*100)%100), 0.1, 0.2);
    // std::stringstream ss; ss << m_pointer_x << "_" << m_pointer_y;
    // Text(0.04, 0.04).DrawText(ss.str(), 0.1, 0.1);
    // Text(0.04, 0.04).DrawText(IntToStr(InPaintingFieldMode()), 0.1, 0.2);
    Text(0.04, 0.04).DrawText(IntToStr(InPaintingEntityMode()), 0.1, 0.1);

    glPushMatrix();
    {
        glTranslated(viewer_x, 0, 0);
        // jednostki
        std::for_each(m_entities.begin(), m_entities.end(), boost::bind(&Entity::Draw, _1));
        // poziom
        m_level_view.SetLevel(m_level, offset);
        m_level_view.Draw(offset);
    }
    glPopMatrix();

    // narysuj pędzel oraz GUI
    glPushAttrib(GL_ENABLE_BIT);
    {
        // wyłącz test głębokości, żeby pędzel oraz gui były zawsze na wierzchu
        glDisable(GL_DEPTH_TEST);
        SetBrush(m_gui->GetActiveBrush());
        if (GetBrush()) {
            glPushMatrix();
            {
                glTranslated(viewer_x, 0, 0);
                Size size(tile_width, tile_height);
                Position position(m_pointer_x * tile_width, m_pointer_y * tile_height);
                if (InPaintingFieldMode()) {
                    // pola zawsze leżą w komórkach siatki
                    position = Position(static_cast<int>(m_pointer_x) * tile_width,
                                        static_cast<int>(m_pointer_y) * tile_height);
                }
                Engine::Get().GetRenderer()->DrawQuad(position, position+size, 1,1,1,.4); // podświetlenie
                GetBrush()->GetSprite()->DrawCurrentFrame(position, size);                // obiekt spod pędzla
            }
            glPopMatrix();
        }
        if (IsGuiVisible()) {
            m_gui->Draw();
        }
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

void Editor::ActionAtCoords(double x, double y) {
    BrushPtr b = m_gui->GetActiveBrush();
    if (b) {
        if (InPaintingFieldMode()) {
            SetFieldAt(x, y, b->GetFieldType());
        } else if (InPaintingEntityMode()) {
            EntityFactory factory;
            std::string name = "mush";
            LevelEntityData entity_data(name, x, y);
            m_entities_to_create.push_back(entity_data);
            m_entities.push_back(factory.CreateEntity(entity_data));
            // std::cout << "New entity: " << name << " " << x << " " << y << std::endl;
        } else if (InPaintingSpecialMode()) {
            std::cout << "Action in special mode" << std::endl;
        }
        else {
            assert(false && "nie odnaleziono trybu rysowania");
        }
    } else {
        ClearFieldAt(x, y);
    }
}

void Editor::ProcessEvents(const SDL_Event& event) {
    if (IsDone()) {
        return;
    }

    if (event.type == SDL_QUIT) {
        SetDone();
    } else if (event.type == SDL_KEYDOWN) {
        if (IsGuiHidden() || m_gui->OnKeyDown(event.key.keysym.sym)==false) {
            m_keys_down[event.key.keysym.sym] = true;
        }
    } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym==SDLK_1) {
            ToggleGui();
        } else if (IsGuiHidden() || m_gui->OnKeyUp(event.key.keysym.sym)==false) {
            m_keys_down[event.key.keysym.sym] = false;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        m_pointer_window_x =       event.motion.x / static_cast<double>(Engine::Get().GetWindow()->GetWidth());
        m_pointer_window_y = 1.0 - event.motion.y / static_cast<double>(Engine::Get().GetWindow()->GetHeight());
        if (IsGuiVisible() && m_gui->OnMouseMove(m_pointer_window_x, m_pointer_window_y)) {
            m_pointer_x = m_pointer_y = 1000;   // przesuń kursor poza ekran (można to zrobić ładniej)
        } else {
            m_pointer_x = MapWindowCoordToWorldX(m_pointer_window_x);
            m_pointer_y = MapWindowCoordToWorldY(m_pointer_window_y);
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        m_pointer_window_x =       event.motion.x / static_cast<double>(Engine::Get().GetWindow()->GetWidth());
        m_pointer_window_y = 1.0 - event.motion.y / static_cast<double>(Engine::Get().GetWindow()->GetHeight());
        if (IsGuiVisible() && m_gui->OnMouseDown(event.button.button, m_pointer_window_x, m_pointer_window_y)) {
        } else {
            m_pointer_x = MapWindowCoordToWorldX(m_pointer_window_x);
            m_pointer_y = MapWindowCoordToWorldY(m_pointer_window_y);
            ActionAtCoords(m_pointer_x, m_pointer_y);
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