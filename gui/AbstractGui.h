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

    // Zwraca true jesli zdarzenie nacisniecia klawisza zostało obsluzone
    virtual bool OnKeyDown(const SDLKey& /* key */) { return false; }
    // Zwraca true jesli zdarzenie zwolnienia klawisza zostało obsluzone
    virtual bool OnKeyUp(const SDLKey& /* key */)   { return false; }
    // Zwraca true jesli zdarzenie przesuniecia kursora myszy zostało obsluzone
    virtual bool OnMouseMove(double /* x */, double /* y */) { return false; }
    // Zwraca true jesli zdarzenie nacisniecia przycisku myszy zostało obsluzone
    virtual bool OnMouseDown(Uint8 /* button */, double /* x */, double /* y */) { return false; }
};


} // namespace gui

#endif
