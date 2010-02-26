#ifndef __LANDCHOICESCREEN_H_INCLUDED__
#define __LANDCHOICESCREEN_H_INCLUDED__

#include "AppState.h"

class LandChoiceScreen: public AppState {
public:
	LandChoiceScreen();
	virtual ~LandChoiceScreen();
    void Init();
    void Start();

    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<AppState> NextAppState() const;

private:
    struct FS {
    	enum FaceState {
    		WALK,
    		STAND,
    	};
    };
    float face_pos_x;      // położenie bohatera na ekranie
    float face_pos_y;

};

#endif
