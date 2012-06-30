#ifndef MAGAME_GUI_H_INCLUDED
#define MAGAME_GUI_H_INCLUDED
#include "StdAfx.h"

namespace gui {

class AbstractGui;
typedef boost::shared_ptr<AbstractGui> AbstractGuiPtr;

class AbstractGui : public boost::enable_shared_from_this<AbstractGui> {
public:
    virtual ~AbstractGui() {}

    virtual void Start() = 0;
    virtual void Init() = 0;
    virtual void Draw() = 0;
    virtual void Update(double dt) = 0;

    virtual bool OnKeyDown(const SDLKey& /* key */) { return false; }
    virtual bool OnKeyUp(const SDLKey& /* key */)   { return false; }
    virtual bool OnMouseMove(double /* x */, double /* y */) { return false; }
    virtual bool OnMouseDown(Uint8 /* button */, double /* x */, double /* y */) { return false; }
};


} // namespace gui

#endif
