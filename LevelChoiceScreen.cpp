#include <SDL/SDL_opengl.h>
#include <boost/assign/std/vector.hpp>     // operator+=()
#include <boost/assign/list_inserter.hpp>  // insert()
#include <cmath>      // fabs
#include <algorithm>  // find
#include <iostream>

#include "Engine.h"
#include "LevelChoiceScreen.h"
#include "MainMenu.h"
#include "Game.h"
#include "Sprite.h"
#include "SpriteConfig.h"
#include "Text.h"


LevelChoiceScreen::LevelChoiceScreen(PlayerPtr player) :
    m_face_pos(0, 0),
    m_current_from_node(0),
    m_current_to_node(0),
    m_sprite(),
    m_tile_width(1.0 / 15),
    m_tile_height(1.0 / 15),
    m_player(player),
    m_next_app_state() {

    srand(time(0));

//
//	0 -------- 1
//	           |
//	           |
//     3 ----- 2
//	           |
//	           |
//	           4

//
//    boost assignment lbrary
//    http://www.boost.org/doc/libs/1_35_0/libs/assign/doc/index.html
//
    using namespace boost::assign;
    m_connections += IntVector(), IntVector(), IntVector(), IntVector(), IntVector();
    m_connections.at(0) += 1;
    m_connections.at(1) += 2, 0;
    m_connections.at(2) += 3, 4, 1;
    m_connections.at(3) += 2;
    m_connections.at(4) += 2;

    // zdefiniuj odwzorowanie (aka mapowanie)  węzeł->nazwa poziomu
    insert(m_node_to_level_name)(0, "1")(1, "2")(2, "2")(3, "1")(4, "2");

//
//    http://en.wikipedia.org/wiki/C%2B%2B0x#Initializer_lists
//    std::vector<int> hello = {1,2,3,4};
//
    m_positions += Point(.1, .7), Point(.7, .7), Point(.7, .4), Point(.3, .4), Point(.7, .1);
    m_face_pos = m_positions.at(0);

    const SpriteConfigData land_choice_sprite_data( DL::Foreground, 8, 0,
                                                    0, 14 * 32, 32, 32, false );
    m_sprite.reset(new Sprite(land_choice_sprite_data));
}

LevelChoiceScreen::~LevelChoiceScreen() {

}

void LevelChoiceScreen::Init() {
    m_next_app_state.reset();
    SetDone(false);
}

void LevelChoiceScreen::Start() {
}

void LevelChoiceScreen::DrawRoad(size_t from, size_t to) const {
    //    fun narysuj_drogę( a, b ):
    //      d = długość drogi
    //      v = czy pozioma
    //      h = czy pionowa
    //
    //      assert (!(v&&h), 'jednocześnie pionowa i pozioma')
    //      frame_id = { zależność id v oraz h}
    //      foreach i in [a+1, b-1]:
    //         m_sprite->DrawCurrentFrame(frame_id)

    Point from_node_pos = m_positions.at(from);
    Point to_node_pos = m_positions.at(to);
//    bool is_vertical = false, is_horizontal = false;
    if (from_node_pos[0] - to_node_pos[0]) {
        if (from_node_pos[0] > to_node_pos[0]) {
            std::swap(from_node_pos, to_node_pos);
        }
        m_sprite->SetCurrentFrame(1);
        m_sprite->DrawCurrentFrame( from_node_pos[0], from_node_pos[1] - m_tile_height / 2,
                                    to_node_pos[0] - from_node_pos[0], m_tile_height );
    } else if (from_node_pos[1] - to_node_pos[1]) {
        if (from_node_pos[1] > to_node_pos[1]) {
            std::swap(from_node_pos, to_node_pos);
        }
        m_sprite->SetCurrentFrame(0);
        m_sprite->DrawCurrentFrame( from_node_pos[0] - m_tile_width / 2, from_node_pos[1],
                                    m_tile_width, to_node_pos[1] - from_node_pos[1] );
    }
}

void LevelChoiceScreen::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // DZIAŁANIE
    // dla każdego węzła i
    //   dla jego każdego sąsiada j
    //     jeżeli i < j:
    //       narysuj drogę z i do j
    //     else:
    //       jeżeli nie istnieje droga z i do j:
    //         narysuj drogę z j do i

    // narysuj drogi
    for (size_t from_node = 0; from_node < m_connections.size(); ++from_node) {
        const IntVector roads = m_connections.at(from_node);
        for (IntVector::const_iterator to_node = roads.begin(); to_node != roads.end(); ++to_node) {
            if (from_node < *to_node) {
                DrawRoad(from_node, *to_node);
            } else if (*to_node < from_node) {
                // drogi dwukierunkowe rysujemy jednokrotnie
                const IntVector cs = m_connections.at(*to_node);
                if (std::find(cs.begin(), cs.end(), from_node) == cs.end()) {
                    DrawRoad(*to_node, from_node);
                }
            }
        }
    }

    // narysuj węzły
    for (size_t node_id = 0; node_id < m_positions.size(); ++node_id) {
        const bool node_enabled = true; // czy można skorzystać z tego węzła
        if (node_enabled) {
            m_sprite->SetCurrentFrame(6);
        } else {
            m_sprite->SetCurrentFrame(7);
        }
        const float x = m_positions[node_id][0] - m_tile_width / 2;
        const float y = m_positions[node_id][1] - m_tile_height / 2;
        m_sprite->DrawCurrentFrame(x, y, m_tile_width, m_tile_height);
//        Engine::Get().Renderer()->DrawSprite(
//                i * 32, 14 * 32, 32, 32,
//                x, y,
//                point_width, point_height, DL::Foreground);
    }

    // narysuj postać
    m_sprite->SetCurrentFrame(8);
    m_sprite->DrawCurrentFrame(m_face_pos[0] - m_tile_width / 2, m_face_pos[1]
            - m_tile_height / 2, m_tile_width, m_tile_height);


    // tekst na górze ekranu
    Text t;
    t.SetSize(.05, .06);
    t.DrawText("WYBIERZ POZIOM", .2, .85);

    //
    SDL_GL_SwapBuffers();
}

// zwraca znak x
int sgn(float x) {
    return x ? (x > 0 ? 1 : -1) : 0;
//    if (x < 0) return -1;
//    if (x > 0) return 1;
//    return 0;
}

int LevelChoiceScreen::NextNode(size_t node) const {
    if (false == m_connections.at(node).empty()) {
        return m_connections.at(node).at(rand() % m_connections.at(node).size());
    }
    return node;
}

bool LevelChoiceScreen::Update(double dt) {
//    if (from_node == to_node) {
//        to_node = NextNode(to_node);
//    }
//    assert(from_node!=to_node && "from == to :/");
//    std::cout << from_node << " " << to_node << std::endl;

    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);
    const float dist_x = to_node_pos.x - m_face_pos[0];
    const float dist_y = to_node_pos.y - m_face_pos[1];
    float vel_x = .5 * sgn(dist_x);
    float vel_y = .4 * sgn(dist_y);

    if (fabs(dist_x) < .01 && fabs(dist_y) < .01) {
        m_current_from_node = m_current_to_node;
        vel_x = vel_y = 0;
        m_face_pos[0] = to_node_pos.x;
        m_face_pos[1] = to_node_pos.y;
    }

    m_face_pos[0] += vel_x * dt;
    m_face_pos[1] += vel_y * dt;

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

    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = m_connections.at(m_current_from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[0] - from_node_pos[0] < 0) {
                // istnieje droga, którą można iść
                m_current_to_node = *it;
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
    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = m_connections.at(m_current_from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[1] - from_node_pos[1] > 0) {
                // istnieje droga, którą można iść
                m_current_to_node = *it;
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
    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = m_connections.at(m_current_from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[1] - from_node_pos[1] < 0) {
                // istnieje droga, którą można iść
                m_current_to_node = *it;
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
    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);

    // czy postać stoi w węźle
    if (m_current_from_node == m_current_to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = m_connections.at(m_current_from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[0] - from_node_pos[0] > 0) {
                // istnieje droga, którą można iść
                m_current_to_node = *it;
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
    if (m_node_to_level_name.find(node)!=m_node_to_level_name.end()) {
        return m_node_to_level_name.at(node);
    }
    return "";
}

void LevelChoiceScreen::RunLevelFromNode() {
    // jeżeli postać jest w drodze (nie stoi w węźle), to nie pozwalamy włączyć poziomu
    // (można zmienić wedle uznania)
    if (m_current_from_node != m_current_to_node) {
        return;
    }

    const std::string level_name = NodeToLevelName(m_current_to_node);
    boost::shared_ptr<LevelChoiceScreen> m_level_choice_screen(shared_from_this());
    Game* game_state = new Game(level_name, m_player);
    game_state->BindLevelChoiceScreen(m_level_choice_screen);
    m_next_app_state.reset(game_state);

    SetDone();
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