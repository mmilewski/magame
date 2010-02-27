#include <SDL/SDL_opengl.h>
#include <boost/assign/std/vector.hpp>  // 'operator+=()'
#include <cmath>  // fabs

#include "Engine.h"
#include "LandChoiceScreen.h"
#include "MainMenu.h"
#include "Game.h"


LandChoiceScreen::LandChoiceScreen() :
    face_pos_x(0),
    face_pos_y(0),
    from_node(0),
    to_node(0),
    m_sprite(),
    m_next_app_state()
    {

    srand(time(0));
//
//	0 -------- 1
//	           |
//	           |
//     3 ----- 2
//	           |
//	           |
//	           4

	using namespace boost::assign;
    connections += IntVector(), IntVector(), IntVector(), IntVector(), IntVector();
    connections.at(0) += 1;
    connections.at(1) += 2, 0;
    connections.at(2) += 3, 4, 1;
    connections.at(3) += 2;
    connections.at(4) += 2;

//
//    http://en.wikipedia.org/wiki/C%2B%2B0x#Initializer_lists
//    std::vector<int> hello = {1,2,3,4};
//

    const int Up = 1, Left = 2, Down = 3, Right = 4;
    moves += BoolVector(), BoolVector(), BoolVector(), BoolVector(), BoolVector();
    moves.at(0) += Right;
    moves.at(1) += Down, Left;
    moves.at(2) += Up, Left, Down;
    moves.at(3) += Right;
    moves.at(4) += Up;

    m_positions += Point( .1, .7 ), Point( .7, .7 ), Point( .7, .4 ), Point( .3, .4 ), Point( .7, .1 );

    face_pos_x = m_positions.at(0)[0];
    face_pos_y = m_positions.at(0)[1];

    const SpriteConfigData land_choice_sprite_data(DL::Foreground, 8, 0, 0, 14 * 32, 32, 32, false);
    m_sprite.reset(new Sprite(land_choice_sprite_data));
}

LandChoiceScreen::~LandChoiceScreen() {

}

void LandChoiceScreen::Init() {
}

void LandChoiceScreen::Start() {
}

#include "Sprite.h"
#include "SpriteConfig.h"

void LandChoiceScreen::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
//	Engine::Get().Renderer()->DrawQuad(.1, .1, .9, .9, 1, 0, 0, 1);


    const float point_width = 1.0/10, point_height = 1.0/10;
//    const int points_count = 5;
//    const Point positions[points_count] = {
//            Point( .1, .7 ),
//            Point( .7, .7 ),
//            Point( .7, .4 ),
//            Point( .3, .4 ),
//            Point( .7, .1 ),
//    };

    // narysuj drogę

    // TOOD: implement
    //  ....

    // narysuj węzły
    for (size_t i = 0; i < m_positions.size(); ++i) {
        m_sprite->SetCurrentFrame(i);
        const float x = m_positions[i][0] - point_width / 2;
        const float y = m_positions[i][1] - point_height / 2;
        m_sprite->DrawCurrentFrame(x, y, point_width, point_height);
//        Engine::Get().Renderer()->DrawSprite(
//                i * 32, 14 * 32, 32, 32,
//                x, y,
//                point_width, point_height, DL::Foreground);
    }

    // narysuj postać
    m_sprite->SetCurrentFrame(8);
    m_sprite->DrawCurrentFrame(face_pos_x-point_width/2, face_pos_y-point_height/2, point_width, point_height);

    SDL_GL_SwapBuffers();
}

// zwraca znak x
int sgn(float x) {
    if (x < 0)  return -1;
    if (x > 0)  return 1;
    return 0;
}

// następny węzeł (dowolny, z którym jest połączenie). Lub node, jeżeli nie ma połączeń
int LandChoiceScreen::NextNode(int node) const {
    if (false == connections.at(node).empty()) {
        return connections.at(node).at(rand()%connections.at(node).size());
    }
    return node;
}

#include <iostream>

bool LandChoiceScreen::Update(double dt) {
//    if (from_node == to_node) {
//        to_node = NextNode(to_node);
//    }
//    assert(from_node!=to_node && "from == to :/");
    std::cout << from_node << " " << to_node << std::endl;

    const Point from_node_pos = m_positions.at(from_node);
    const Point to_node_pos = m_positions.at(to_node);
    const float dist_x = to_node_pos.x - face_pos_x;
    const float dist_y = to_node_pos.y - face_pos_y;
    float vel_x = .3 * sgn(dist_x);
    float vel_y = .2 * sgn(dist_y);

    if(fabs(dist_x)<.01 && fabs(dist_y)<.01) {
        from_node = to_node;
        vel_x = vel_y = 0;
        face_pos_x = to_node_pos.x;
        face_pos_y = to_node_pos.y;
    }

    face_pos_x += vel_x * dt;
    face_pos_y += vel_y * dt;

    return !IsDone();
}

bool LandChoiceScreen::GoLeft() {
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
    const Point from_node_pos = m_positions.at(from_node);
    const Point to_node_pos = m_positions.at(to_node);

    // czy postać stoi w węźle
    if (from_node == to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = connections.at(from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[0] - from_node_pos[0] < 0) {
                // istnieje droga, którą można iść
                to_node = *it;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w prawo
    if (to_node_pos[0] - from_node_pos[0] > 0) {
        std::swap(from_node, to_node);
        return true;
    }
    return false;
}

bool LandChoiceScreen::GoUpward() {
    const Point from_node_pos = m_positions.at(from_node);
    const Point to_node_pos = m_positions.at(to_node);

    // czy postać stoi w węźle
    if (from_node == to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = connections.at(from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[1] - from_node_pos[1] > 0) {
                // istnieje droga, którą można iść
                to_node = *it;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w lewo
    if (to_node_pos[1] - from_node_pos[1] < 0) {
        std::swap(from_node, to_node);
        return true;
    }
    return false;
}

bool LandChoiceScreen::GoDown() {
    const Point from_node_pos = m_positions.at(from_node);
    const Point to_node_pos = m_positions.at(to_node);

    // czy postać stoi w węźle
    if (from_node == to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = connections.at(from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[1] - from_node_pos[1] < 0) {
                // istnieje droga, którą można iść
                to_node = *it;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w prawo
    if (to_node_pos[1] - from_node_pos[1] > 0) {
        std::swap(from_node, to_node);
        return true;
    }
    return false;
}

bool LandChoiceScreen::GoRight() {
    const Point from_node_pos = m_positions.at(from_node);
    const Point to_node_pos = m_positions.at(to_node);

    // czy postać stoi w węźle
    if (from_node == to_node) {
        // czy istnieje droga w lewo
        IntVector dst_nodes = connections.at(from_node); // połączenia z from_node
        for (IntVector::const_iterator it = dst_nodes.begin(); it != dst_nodes.end(); ++it) {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[0] - from_node_pos[0] > 0) {
                // istnieje droga, którą można iść
                to_node = *it;
                return true;
            }
        }
        return false;
    }

    // czy postać idzie w lewo
    if (to_node_pos[0] - from_node_pos[0] < 0) {
        std::swap(from_node, to_node);
        return true;
    }
    return false;
}

void LandChoiceScreen::RunLevelFromNode() {
    if (from_node != to_node) {
        return;
    }
//    m_next_app_state = boost::shared_ptr<AppState>(new Game("1", PlayerPtr()));
    m_next_app_state = boost::shared_ptr<AppState>(new Game("1"));
    SetDone();
}

void LandChoiceScreen::ProcessEvents(const SDL_Event & event) {
    if (event.type == SDL_QUIT) {
        SetDone();
    }

    if (event.type == SDL_KEYDOWN) {
        SDLKey key = event.key.keysym.sym;
        if (key == SDLK_ESCAPE) {
            SetDone();
        } else if (key == SDLK_LEFT) {
            GoLeft();
        } else if (key == SDLK_RIGHT) {
            GoRight();
        } else if (key == SDLK_UP) {
            GoUpward();
        } else if (key == SDLK_DOWN) {
            GoDown();
        } else if( key==SDLK_RETURN) {
            RunLevelFromNode();
        }
    }
}

boost::shared_ptr<AppState> LandChoiceScreen::NextAppState() const {
    //
    // TODO: zaimplementować zwracanie następnego stanu
    //

//    return boost::shared_ptr<AppState>(new MainMenu);
    return m_next_app_state;
}

