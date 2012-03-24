#ifndef MAGAME_TWIN_SHOT_UPGRADE_H_INCLUDED
#define MAGAME_TWIN_SHOT_UPGRADE_H_INCLUDED
#include "StdAfx.h"

#include "entity/Entity.h"

class TwinShotUpgrade : public Entity {
public:
    TwinShotUpgrade(double x, double y)
        : Entity(x,y, 0, 0) {
    }

    ET::EntityType GetType() const { return ET::TwinShot; }
};

#endif
