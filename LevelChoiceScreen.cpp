#include "StdAfx.h"

#include "Engine.h"
#include "LevelChoiceScreen.h"
#include "MainMenu.h"
#include "Game.h"
#include "video/Sprite.h"
#include "video/SpriteConfig.h"
#include "video/Text.h"
#include "common/Utils.h"


LevelChoiceScreen::LevelChoiceScreen(PlayerPtr player) :
    m_face_pos(0, 0),
    m_current_from_node(0),
    m_current_to_node(0),
    m_horizontal_road_sprite(),
    m_vertical_road_sprite(),
    m_entry_enabled_sprite(),
    m_entry_disabled_sprite(),
    m_face_sprite(),
    m_tile_size(1.0 / 15, 1.0 / 15),
    m_player(player),
    m_next_app_state() {

//
//    0 -------- 1
//               |
//               |
//       3 ----- 2
//               |
//               |
//               4
    m_positions = { Position(.1, .7), Position(.7, .7), Position(.7, .4), Position(.3, .4), Position(.7, .1) };
    m_face_pos = m_positions.at(0);

    m_connections.resize(5, IntVector());
    m_connections[0] = { 1 };
    m_connections[1] = { 2, 0 };
    m_connections[2] = { 3, 4, 1 };
    m_connections[3] = { 2 };
    m_connections[4] = { 2 };

    // zdefiniuj odwzorowanie (aka mapowanie)  węzeł->nazwa poziomu
    m_node_to_level_name = { {0, "1"}, {1, "2"}, {2, "3"}, {3, "4"}, {4, "5"} };

    // załaduj sprite'y
    const SpriteConfigData horizontal_road_data(DL::Foreground, 1, 1, 0 * 32, 17 * 32, 32, 32, false, false);
    const SpriteConfigData vertical_road_data(DL::Foreground, 1, 1,  1 * 32, 17 * 32, 32, 32, false, false);
    const SpriteConfigData entry_enabled_data(DL::Foreground, 4, .1,  4 * 32, 17 * 32, 32, 32, true, false);
    const SpriteConfigData face_data(DL::Foreground, 4, .1,  0 * 32, 23 * 32, 32, 32, true, false);

    m_horizontal_road_sprite.reset(new Sprite(horizontal_road_data));
    m_vertical_road_sprite.reset(new Sprite(vertical_road_data));
    m_entry_enabled_sprite.reset(new Sprite(entry_enabled_data));
    m_entry_disabled_sprite.reset(new Sprite(entry_enabled_data)); // ! pełna kopia, a nie kopia wskaźnika
    m_face_sprite.reset(new Sprite(face_data));
}

LevelChoiceScreen::~LevelChoiceScreen() {

}

void LevelChoiceScreen::Init() {
    m_next_app_state.reset();
    SetDone(false);
}

void LevelChoiceScreen::Start() {
}

void LevelChoiceScreen::DrawRoad(size_t src_point_index, size_t tgt_point_index) const {
    //    fun narysuj_drogę( a, b ):
    //      d = długość drogi
    //      v = czy pozioma
    //      h = czy pionowa
    //
    //      assert (!(v&&h), 'jednocześnie pionowa i pozioma')
    //      frame_id = { zależność id v oraz h}
    //      foreach i in [a+1, b-1]:
    //         m_sprite->DrawCurrentFrame(frame_id)

    Position from_node_pos = m_positions.at(src_point_index);
    Position to_node_pos = m_positions.at(tgt_point_index);
    // jeśli droga jest pionowa
    if (from_node_pos[0] - to_node_pos[0]) {
        if (from_node_pos[0] > to_node_pos[0]) {
            std::swap(from_node_pos, to_node_pos);
        }
        m_vertical_road_sprite->SetRepeat(m_tile_size);
        m_vertical_road_sprite->DrawCurrentFrame(
                    Position(from_node_pos[0],             from_node_pos[1] - m_tile_size.Y()/2),
                    Size((to_node_pos-from_node_pos).X(),  m_tile_size.Y()));
    }
    // jeśli droga jest pozioma
    else if (from_node_pos[1] - to_node_pos[1]) {
        if (from_node_pos[1] > to_node_pos[1]) {
            std::swap(from_node_pos, to_node_pos);
        }
        m_horizontal_road_sprite->SetRepeat(m_tile_size);
        m_horizontal_road_sprite->DrawCurrentFrame(
                    Position(from_node_pos[0] - m_tile_size[0]/2,  from_node_pos[1]),
                    Size(m_tile_size[0],                           (to_node_pos - from_node_pos).Y()));
    }
}

void LevelChoiceScreen::DrawAllRoads() const {
    // DZIAŁANIE
    // dla każdego węzła i
    //   dla jego każdego sąsiada j
    //     jeżeli i < j:
    //       narysuj drogę z i do j
    //     else:
    //       jeżeli nie istnieje droga z i do j:
    //         narysuj drogę z j do i

    size_t from_node = 0;
    for(const IntVector& roads : m_connections) {
        for(size_t to_node : roads) {
            if (from_node < to_node) {
                DrawRoad(from_node, to_node);
            }
        }
        ++from_node;
    }
}

void LevelChoiceScreen::DrawAllNodes() const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(const Position& node_pos : m_positions) {
        bool node_enabled = true; // czy można skorzystać z tego węzła (na razie wszystkie węzły są aktywne)
        (node_enabled ? m_entry_enabled_sprite : m_entry_disabled_sprite)
                ->DrawCurrentFrame(node_pos - m_tile_size/2, m_tile_size);
    }
    glDisable(GL_BLEND);
}

void LevelChoiceScreen::DrawFace() const {
    m_face_sprite->DrawCurrentFrame(m_face_pos - m_tile_size/2, m_tile_size);

    // // Aby zobaczyć jak działa powtarzanie sprite'ów odkomentuj poniższy kawałek kodu
    // // narysuj postać
    // m_face_sprite->SetRepeat(m_tile_width, m_tile_width);
    // m_face_sprite->DrawCurrentFrame(m_face_pos[0] - m_tile_width / 2,
    //                                 m_face_pos[1] - m_tile_height / 2,
    //                                 m_tile_width * 3.5,
    //                                 m_tile_height * 2.75);
}

void LevelChoiceScreen::DrawHud() const {
    // tekst na górze ekranu
    Text t;
    t.SetSize(.05, .06);
    t.DrawText("WYBIERZ POZIOM", .2, .85);
}

void LevelChoiceScreen::Draw() const {
    DrawAllRoads();
    DrawAllNodes();
    DrawFace();
    DrawHud();
}

namespace {
// zwraca znak x
int sgn(double x) {
    return x ? (x > 0 ? 1 : -1) : 0;
}
}

bool LevelChoiceScreen::Update(double dt) {
    // uaktualnij położenie twarzy postaci
    Velocity face_velocity = Vector2(.6, .5);
    const Position to_node_pos = m_positions.at(m_current_to_node);
    const Vector2 distance = to_node_pos - m_face_pos;
    face_velocity = face_velocity.Scale(distance.Map(sgn));

    // sprawdź czy postać należy zatrzymać (bo jest w węźle)
    if (fabs(distance.X()) < .01 && fabs(distance.Y()) < .01) {
        m_current_from_node = m_current_to_node;
        face_velocity = Velocity::ZERO;
        m_face_pos = to_node_pos;
    }

    // uaktualnij położenie na podstawie prędkości
    m_face_pos += face_velocity * dt;

    // uaktualnij animacje
    m_horizontal_road_sprite->Update(dt);
    m_vertical_road_sprite->Update(dt);
    m_entry_enabled_sprite->Update(dt);
    m_face_sprite->Update(dt);

    return !IsDone();
}

bool LevelChoiceScreen::GoLeft() {
    // DZIAŁANIE
    //    jeżeli postać stoi w węźle
    //        sprawdź czy istnieje droga w lewo
    //        jeżeli tak
    //            ustaw to_node jako jej koniec
    //            zwróć true
    //        jeżeli nie
    //             zwróć false
    //    jeżeli postać idzie w prawo
    //        niech idzie w lewo
    //        return true
    //    return false
    //

    const Position from_node_pos = m_positions.at(m_current_from_node);
    const Position to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        for(size_t to_node : m_connections.at(m_current_from_node)) {  // przejrzyj połączenia z from_node
            Position connection_node_pos = m_positions.at(to_node);
            if (connection_node_pos[0] - from_node_pos[0] < 0) {
                // istnieje droga, którą można iść
                m_current_to_node = to_node;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w prawo
    if (to_node_pos[0] - from_node_pos[0] > 0) {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

bool LevelChoiceScreen::GoUpward() {
    const Position from_node_pos = m_positions.at(m_current_from_node);
    const Position to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        for(size_t to_node : m_connections.at(m_current_from_node)) {  // przejrzyj połączenia z from_node
            Position connection_node_pos = m_positions.at(to_node);
            if (connection_node_pos[1] - from_node_pos[1] > 0) {
                // istnieje droga, którą można iść
                m_current_to_node = to_node;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w lewo
    if (to_node_pos[1] - from_node_pos[1] < 0) {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

bool LevelChoiceScreen::GoDown() {
    const Position from_node_pos = m_positions.at(m_current_from_node);
    const Position to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        for(size_t to_node : m_connections.at(m_current_from_node)) {  // przejrzyj połączenia z from_node
            Position connection_node_pos = m_positions.at(to_node);
            if (connection_node_pos[1] - from_node_pos[1] < 0) {
                // istnieje droga, którą można iść
                m_current_to_node = to_node;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w prawo
    if (to_node_pos[1] - from_node_pos[1] > 0) {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

bool LevelChoiceScreen::GoRight() {
    const Position from_node_pos = m_positions.at(m_current_from_node);
    const Position to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        for(size_t to_node : m_connections.at(m_current_from_node)) {  // przejrzyj połączenia z from_node
            Position connection_node_pos = m_positions.at(to_node);
            if (connection_node_pos[0] - from_node_pos[0] > 0) {
                // istnieje droga, którą można iść
                m_current_to_node = to_node;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w lewo
    if (to_node_pos[0] - from_node_pos[0] < 0) {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

std::string LevelChoiceScreen::NodeToLevelName(int node) {
    return m_node_to_level_name.count(node) ? m_node_to_level_name.at(node) : "";
}

void LevelChoiceScreen::RunLevelFromNode() {
    // jeżeli postać jest w drodze (nie stoi w węźle), to nie pozwalamy włączyć poziomu
    // (można zmienić wedle uznania)
    if (m_current_from_node != m_current_to_node) {
        return;
    }

    const std::string level_name = NodeToLevelName(m_current_to_node);
    if (level_name.empty()) {
        std::cerr << "WARNING: this node hasn't any level assigned.\n";
    } else {
        Game* game_state = new Game(level_name, m_player);
        game_state->BindLevelChoiceScreen(shared_from_this());
        m_next_app_state.reset(game_state);

        SetDone();
    }
}

void LevelChoiceScreen::ProcessEvents(const SDL_Event & event) {
    if (event.type == SDL_QUIT) {
        SetDone();
    }

    if (event.type == SDL_KEYDOWN) {
        SDLKey key = event.key.keysym.sym;
        if (key == SDLK_ESCAPE) {
            m_next_app_state = AppStatePtr(new MainMenu());
            SetDone();
        } else if (key == SDLK_LEFT) {
            GoLeft();
        } else if (key == SDLK_RIGHT) {
            GoRight();
        } else if (key == SDLK_UP) {
            GoUpward();
        } else if (key == SDLK_DOWN) {
            GoDown();
        } else if (key == SDLK_RETURN) {
            RunLevelFromNode();
        }
    }
}

boost::shared_ptr<AppState> LevelChoiceScreen::NextAppState() const {
    return m_next_app_state;
}

void LevelChoiceScreen::SetPlayer(PlayerPtr player) {
    m_player = player;
}
