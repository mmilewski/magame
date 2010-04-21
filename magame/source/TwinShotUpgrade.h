#ifndef __TWIN_SHOT_UPGRADE_H_INCLUDED__
#define __TWIN_SHOT_UPGRADE_H_INCLUDED__
#include "StdAfx.h"

#include "Entity.h"

class TwinShotUpgrade : public Entity {
public:
    TwinShotUpgrade(double x, double y)
        : Entity(x,y, 0, 0) {
    }

    ET::EntityType GetType() const { return ET::TwinShot; }
};

#endif
