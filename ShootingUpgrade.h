#ifndef __SHOOTING_UPGRADE_H_INCLUDED__
#define __SHOOTING_UPGRADE_H_INCLUDED__
#include "StdAfx.h"

#include "Entity.h"

class ShootingUpgrade : public Entity {
public:
    ShootingUpgrade(double x, double y)
        : Entity(x,y, 0, 0) {
    }

    ET::EntityType GetType() const { return ET::SingleShot; }
};

#endif
