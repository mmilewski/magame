#ifndef MAGAME_GUI_H_INCLUDED
#define MAGAME_GUI_H_INCLUDED
#include "StdAfx.h"

class Gui;
typedef boost::shared_ptr<Gui> GuiPtr;

class Gui : public boost::enable_shared_from_this<Gui> {
public:
    virtual ~Gui() {}

    virtual void Start() = 0;
    virtual void Init() = 0;
    virtual void Draw() = 0;
    virtual void Update(double dt) = 0;

    virtual bool OnKeyDown(const SDLKey& /* key */) { return false; }
    virtual bool OnKeyUp(const SDLKey& /* key */)   { return false; }
    virtual bool OnMouseMove(double /* x */, double /* y */) { return false; }
    virtual bool OnMouseDown(Uint8 /* button */, double /* x */, double /* y */) { return false; }
};

#endif
