#include <SDL/SDL_opengl.h>
#include <boost/assign/std/vector.hpp> // for 'operator+=()'

#include "Engine.h"
#include "LandChoiceScreen.h"
#include "MainMenu.h"


LandChoiceScreen::LandChoiceScreen() :
	AppState(),
	face_pos_x(.1),
	face_pos_y(.5)
	{

//
//	0 -------- 1
//	           |
//	           |
//	  3 ------ 2
//	           |
//	           |
//	           4

//	float positions[][2] = {
//			{ .1, .1 },
//			{ .6, .1 },
//			{ .6, .4 },
//			{ .2, .4 },
//			{ .6, .7 },
//	};

	using namespace boost::assign;
	typedef std::vector<int> IntVector;
	std::vector< IntVector > connections;
	connections += IntVector(), IntVector(), IntVector(), IntVector(), IntVector();
	connections.at(0) += 1;
	connections.at(1) += 0, 2;
	connections.at(2) += 1, 3, 4;
	connections.at(3) += 2;
	connections.at(4) += 2;

	typedef std::vector<bool> BoolVector;
	const int Up=1, Left=2, Down=3, Right=4;
	std::vector< BoolVector > moves;
	moves += BoolVector(), BoolVector(), BoolVector(), BoolVector(), BoolVector();
	moves.at(0) += Right;
	moves.at(1) += Down, Left;
	moves.at(2) += Up, Left, Down;
	moves.at(3) += Right;
	moves.at(4) += Up;
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

	const SpriteConfigData land_choice_sprite_data(DL::Foreground, 6, 0, 0, 14*32, 32, 32, false);

	const float point_width = .1, point_height = .1;
	const int points_count = 5;
	float positions[points_count][2] = {
			{ .2, .7 },
			{ .7, .7 },
			{ .7, .4 },
			{ .3, .4 },
			{ .7, .1 },
	};

	const Sprite sprite(land_choice_sprite_data);
	for (int i = 0; i < points_count; ++i) {
		sprite.SetCurrentFrame(i);
		const float x = positions[i][0] - point_width/2;
		const float y = positions[i][1] - point_height/2;
		sprite.DrawCurrentFrame(x, y, point_width, point_height);
//		Engine::Get().Renderer()->DrawSprite(
//				i*32, 14*32, 32, 32,
//				x, y, point_width, point_height,
//				DL::Foreground );
	}

	SDL_GL_SwapBuffers();
}

bool LandChoiceScreen::Update(double dt) {
	return !IsDone();
}

void LandChoiceScreen::ProcessEvents(const SDL_Event & event) {
	if (event.type == SDL_QUIT) {
		SetDone();
	}

	if (event.type == SDL_KEYDOWN) {
		SDLKey key = event.key.keysym.sym;
		if (key == SDLK_ESCAPE) {
			SetDone();
		}
	}
}

boost::shared_ptr<AppState> LandChoiceScreen::NextAppState() const {
	//
	// TODO: zaimplementować zwracanie następnego stanu
	//

	return boost::shared_ptr<AppState>(new MainMenu);
}

