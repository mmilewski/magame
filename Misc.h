#ifndef __MISC_H_INCLUDED__
#define __MISC_H_INCLUDED__
#include "StdAfx.h"

#include "Entity.h"

class Orb : public Entity {
public:
    Orb(double x, double y)
        : Entity(x,y, 0, 0) {
    }

    ET::EntityType GetType() const { return ET::Orb; }
};

class HigherJumpUpgrade : public Entity {
public:
    HigherJumpUpgrade(double x, double y)
        : Entity(x,y, 0, 0) {
    }

    ET::EntityType GetType() const { return ET::HigherJump; }
};

#endif
