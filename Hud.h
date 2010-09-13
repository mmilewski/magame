#ifndef __HUD_H_INCLUDED__
#define __HUD_H_INCLUDED__
#include "StdAfx.h"


class Hud;
typedef boost::shared_ptr<Hud> HudPtr;

class Hud : public boost::enable_shared_from_this<Hud> {
public:
    explicit Hud() {
    }

    virtual ~Hud() {}

    virtual void Start() = 0;
    virtual void Init() = 0;
    virtual void Draw() = 0;
    virtual void Update(double dt) = 0;

    virtual bool OnKeyDown(const SDLKey& key) { return false; }
    virtual bool OnKeyUp(const SDLKey& key)   { return false; }
    virtual bool OnMouseMove(double x, double y) { return false; }
    virtual bool OnMouseDown(Uint8 button, double x, double y) { return false; }
};


class EditorHud : public Hud, public boost::enable_shared_from_this<EditorHud> {
public:
    void Start() {}
    void Init() {}
    void Draw() {}
    void Update(double dt) {}
};



#endif
